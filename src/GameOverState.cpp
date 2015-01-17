/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

Crush - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <GameOverState.hpp>
#include <Game.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
    sf::RectangleShape backgroundRect;
    const float waitTime = 2.f;
    const std::string music = "res/sound/music/game_over.ogg";

    //point consts
    const sf::Uint16 pointsPerLife = 300u;
    const sf::Uint16 pointsPerHat = 100u;
    const float hatBonus = 1000.f;

    //animation consts
    const float maxFrameRate = 18.f;
    const sf::Vector2i frameSize(41, 64);
    const float spriteScale = 2.f;
    Animation idle(2, 2);
    Animation run(0, 5);

    //layout positions
    const sf::Vector2f playerOnePosition(60.f, 280.f);
    const sf::Vector2f playerTwoPosition(60.f, 680.f);
    const sf::Vector2f barOnePosition = playerOnePosition + sf::Vector2f(120.f, 60.f);
    const sf::Vector2f barTwoPosition = playerTwoPosition + sf::Vector2f(120.f, 60.f);
    const sf::Vector2f playerOneCrownPos = barOnePosition + sf::Vector2f(1550.f, -20.f);
    const sf::Vector2f playerTwoCrownPos = barTwoPosition + sf::Vector2f(1550.f, -20.f);

    const float maxWidth = 1920.f - (barOnePosition.x * 2.f);
}

GameOverState::GameOverState(StateStack& stack, Context context)
    : State                 (stack, context),
    m_playerOneScoreText    (std::to_string(context.gameData.playerOne.score), context.gameInstance.getFont("res/fonts/VeraMono.ttf"), 40u),
    m_playerTwoScoreText    (std::to_string(context.gameData.playerTwo.score), context.gameInstance.getFont("res/fonts/VeraMono.ttf"), 40u),
    m_statusText            ("", context.gameInstance.getFont("res/fonts/VeraMono.ttf"), 40u),
    m_playerOneRunningScore (static_cast<float>(context.gameData.playerOne.score)),
    m_playerTwoRunningScore (static_cast<float>(context.gameData.playerTwo.score)),
    m_barIndex              (0u),
    m_hatIcon               (context.gameInstance.getTextureResource().get("res/textures/ui/hat_bonus.png")),
    m_drawingEnabled        (false),
    m_animateBars           (false),
    m_doHatBonus            (false),
    m_canContinue           (false)
{
    context.renderWindow.setView(context.defaultView);

    backgroundRect.setFillColor({ 0u, 0u, 0u, 198u });
    backgroundRect.setSize(context.defaultView.getSize());

    //context.gameInstance.playMusic(music);

    //parse context data and sum up scores
    auto& tr = context.gameInstance.getTextureResource();
    auto& gameData = context.gameData;

    m_playerOneSprite.setTexture(tr.get("res/textures/characters/playerOne_diffuse.png"));
    if (!gameData.playerOne.enabled) m_playerOneSprite.setColour({ 255u, 255u, 255u, 120u });
    initSprite(m_playerOneSprite, playerOnePosition);

    m_playerTwoSprite.setTexture(tr.get("res/textures/characters/playerTwo_diffuse.png"));
    if (!gameData.playerTwo.enabled) m_playerTwoSprite.setColour({ 255u, 255u, 255u, 120u });
    initSprite(m_playerTwoSprite, playerTwoPosition);
    
    //--------------------------------
    auto& font = context.gameInstance.getFont("res/fonts/VeraMono.ttf");
    m_texts.emplace_back("RESULTS", font, 80u);
    auto& title = m_texts.back();
    Util::Position::centreOrigin(title);
    title.setPosition(context.renderWindow.getView().getCenter());
    title.move(0.f, -500.f);

    if (gameData.playerOne.enabled)
    {
        m_texts.emplace_back(gameData.playerOne.name, font, 40u);
        auto& playerOneName = m_texts.back();
        playerOneName.setPosition(playerOnePosition);
        playerOneName.move(0.f, -80.f);
    }

    if (gameData.playerTwo.enabled)
    {
        m_texts.emplace_back(gameData.playerTwo.name, font, 40u);
        auto& playerTwoName = m_texts.back();
        playerTwoName.setPosition(playerTwoPosition);
        playerTwoName.move(0.f, -80.f);
    }

    m_playerOneScoreText.setPosition(playerOnePosition);
    m_playerOneScoreText.move(0.f, 140.f);

    m_playerTwoScoreText.setPosition(playerTwoPosition);
    m_playerTwoScoreText.move(0.f, 140.f);

    Util::Position::centreOrigin(m_statusText);
    m_statusText.setPosition(context.renderWindow.getView().getCenter());

    //--------------------------------

    float playerOneLifeScore = pointsPerLife * static_cast<float>(gameData.playerOne.lives);
    float playerOneHatScore = pointsPerHat * gameData.playerOne.hatTime;
    float playerOneTotalScore = /*context.gameData.playerOne.score + */playerOneLifeScore + playerOneHatScore;
    //if (context.gameData.playerOne.hasHat) playerOneTotalScore += hatBonus;

    float playerTwoLifeScore = pointsPerLife * static_cast<float>(gameData.playerTwo.lives);
    float playerTwoHatScore = pointsPerHat * gameData.playerTwo.hatTime;
    float playerTwoTotalScore = /*context.gameData.playerTwo.score + */playerTwoLifeScore + playerTwoHatScore;
    //if (context.gameData.playerTwo.hasHat) playerTwoTotalScore += hatBonus;

    float pxPerPoint = maxWidth / std::max(playerOneTotalScore, playerTwoTotalScore);
    m_playerOneBar.emplace_back(sf::Color::Red, pxPerPoint * playerOneLifeScore, playerOneLifeScore);
    m_playerOneBar.back().setPosition(barOnePosition);
    m_playerOneBar.back().setTexture(tr.get("res/textures/ui/bar_shadow_red.png"));
    m_playerOneBar.back().setTitleString("Lives Remaining Bonus");
    m_playerOneBar.emplace_back(sf::Color::Yellow, pxPerPoint * playerOneHatScore, playerOneHatScore);
    m_playerOneBar.back().setPosition(barOnePosition);
    m_playerOneBar.back().setTexture(tr.get("res/textures/ui/bar_shadow_yellow.png"));
    m_playerOneBar.back().setTitleString("Hat Time Bonus");
    m_playerOneBar.back().move(pxPerPoint* playerOneLifeScore, 0.f);

    m_playerTwoBar.emplace_back(sf::Color::Red, pxPerPoint * playerTwoLifeScore, playerTwoLifeScore);
    m_playerTwoBar.back().setPosition(barTwoPosition);
    m_playerTwoBar.back().setTexture(tr.get("res/textures/ui/bar_shadow_red.png"));
    m_playerTwoBar.emplace_back(sf::Color::Yellow, pxPerPoint * playerTwoHatScore, playerTwoHatScore);
    m_playerTwoBar.back().setPosition(barTwoPosition);
    m_playerTwoBar.back().setTexture(tr.get("res/textures/ui/bar_shadow_yellow.png"));
    m_playerTwoBar.back().move(pxPerPoint * playerTwoLifeScore, 0.f);

    if (gameData.playerOne.hasHat)
        m_hatIcon.setPosition(playerOneCrownPos);
    else if (gameData.playerTwo.hasHat)
        m_hatIcon.setPosition(playerTwoCrownPos);

    m_hatIcon.setTransparency(0.f);


    //-------------------------------
    TimedEvent te;
    te.time = waitTime;
    te.action = [this]()
    {
        m_drawingEnabled = true;
    };
    m_timedEvents.push_back(te);

    te.time++;
    te.action = std::bind(&GameOverState::animateBar, this);
    m_timedEvents.push_back(te);

    te.time = 7.f;
    te.action = [&, this]()
    {
        m_texts.emplace_back("Press C to Continue", font, 40u);
        auto& t = m_texts.back();
        Util::Position::centreOrigin(t);
        t.setPosition(getContext().renderWindow.getView().getCenter());
        t.move(0.f, 480.f);
        m_canContinue = true;
    };
    m_timedEvents.push_back(te);
}

GameOverState::~GameOverState()
{
    //getContext().gameInstance.stopMusic();// playMusic("res/sound/music/intro.ogg");
}

bool GameOverState::update(float dt)
{
    for (auto& e : m_timedEvents)
        e.update(dt);

    m_timedEvents.remove_if([](const TimedEvent& e){ return e.expired; });

    if (m_drawingEnabled)
    {
        m_playerOneSprite.update(dt);
        m_playerTwoSprite.update(dt);
        m_hatIcon.update(dt);

        if (m_animateBars)
        {
            bool barOneDone = m_playerOneBar[m_barIndex].update(dt);
            bool barTwoDone = m_playerTwoBar[m_barIndex].update(dt);

            //m_playerOneRunningScore = m_playerOneBar[m_barIndex].getValue();
            m_playerOneScoreText.setString(std::to_string(static_cast<int>(m_playerOneRunningScore + m_playerOneBar[m_barIndex].getValue())));
            //m_playerTwoRunningScore = m_playerTwoBar[m_barIndex].getValue();
            m_playerTwoScoreText.setString(std::to_string(static_cast<int>(m_playerTwoRunningScore + m_playerTwoBar[m_barIndex].getValue())));

            if (barOneDone) m_playerOneSprite.play(idle);
            if (barTwoDone) m_playerTwoSprite.play(idle);

            if (barOneDone && barTwoDone)
            {
                m_playerOneRunningScore += m_playerOneBar[m_barIndex].getValue();
                m_playerTwoRunningScore += m_playerTwoBar[m_barIndex].getValue();

                m_barIndex++;
                m_animateBars = false;

                if (m_barIndex < m_playerOneBar.size())
                {
                    TimedEvent te;
                    te.time = 0.8f;
                    te.action = std::bind(&GameOverState::animateBar, this);
                    m_timedEvents.push_back(te);
                }
                else
                {
                    TimedEvent te;
                    te.time = 0.8f;
                    te.action = [this]()
                    {
                        if(getContext().gameData.playerOne.hasHat)
                        {
                            m_playerOneRunningScore += hatBonus;
                            m_playerOneScoreText.setString(std::to_string(static_cast<int>(m_playerOneRunningScore)));
                            m_doHatBonus = true;
                        }
                        else if( getContext().gameData.playerTwo.hasHat)
                        {
                            m_playerTwoRunningScore += hatBonus;
                            m_playerTwoScoreText.setString(std::to_string(static_cast<int>(m_playerTwoRunningScore)));
                            m_doHatBonus = true;
                        }

                        if (m_doHatBonus)
                        {
                            m_statusText.setString("Hat Saviour!");
                            Util::Position::centreOrigin(m_statusText);
                        }
                    };
                    m_timedEvents.push_back(te);
                }
            }
        }

        else if (m_doHatBonus)
        {
            //do crown icon anim if someone has hat
            //and falsify when complete
            m_doHatBonus = !m_hatIcon.fadeIn(dt);
        }
    }

    return true; //return true so we can see remaining baddies bouncing about / finish death animations
}

void GameOverState::draw()
{      
    if (m_drawingEnabled)
    {
        auto& rw = getContext().renderWindow;
        rw.draw(backgroundRect);

        if (getContext().gameData.playerOne.enabled)
        {
            rw.draw(m_playerOneSprite);
            for (const auto& b : m_playerOneBar)
            {
                rw.draw(b);
            }
            rw.draw(m_playerOneScoreText);
        }

        if (getContext().gameData.playerTwo.enabled)
        {
            rw.draw(m_playerTwoSprite);
            for (const auto& b : m_playerTwoBar)
            {
                rw.draw(b);
            }
            rw.draw(m_playerTwoScoreText);
        }

        rw.draw(m_hatIcon);

        for (const auto& t : m_texts)
        {
            rw.draw(t);
        }
        rw.draw(m_statusText);
    }
}

bool GameOverState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        if (evt.key.code == sf::Keyboard::C)
        {
            continueGame();           
        }
    }

    else if (evt.type == sf::Event::JoystickButtonPressed)
    {
        if (evt.joystickButton.button == 7)
        {
            continueGame();
        }
    }
    return false;
}

//private
void GameOverState::initSprite(AnimatedSprite& sprite, const sf::Vector2f& position)
{
    //m_playerOneSprite.setNormalMap(tr.get("res/textures/player_normal.png"));
    //m_playerOneSprite.setShader(shader); //using shader needs scene lighting
    sprite.setFrameCount(8u);
    sprite.setFrameRate(maxFrameRate);
    sprite.setFrameSize(frameSize);
    sprite.setLooped(true);
    sprite.play(idle);
    sprite.setScale(spriteScale, spriteScale);
    sprite.setPosition(position);
}

void GameOverState::continueGame()
{
    if (!m_canContinue) return;

    getContext().gameData.playerOne.score = static_cast<sf::Uint32>(m_playerOneRunningScore);
    getContext().gameData.playerTwo.score = static_cast<sf::Uint32>(m_playerTwoRunningScore);

    requestStackClear();
    auto& gameData = getContext().gameData;
    if (++gameData.mapIndex == gameData.mapList.size() //reached the end of the map list
        || (!gameData.playerOne.enabled && !gameData.playerTwo.enabled)) //both players are dead
    {
        //go back to main menu
        requestStackPush(States::ID::Menu);
    }
    else
    {
        //load next map
        requestStackPush(States::ID::Game);
    }
}

void GameOverState::animateBar()
{
    m_animateBars = true;
    m_playerOneSprite.play(run);
    m_playerTwoSprite.play(run);
    m_statusText.setString(m_playerOneBar[m_barIndex].getTitleString());
    Util::Position::centreOrigin(m_statusText);
}