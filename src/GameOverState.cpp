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
    const sf::Uint16 pointsPerhat = 100u;
    const sf::Uint16 hatBonus = 1000u;

    //animation consts
    const float maxFrameRate = 12.f;
    const sf::Vector2i frameSize(41, 64);
    const float spriteScale = 2.f;
    Animation idle(2, 2);
    Animation run(0, 5);

    //layout positions
    const sf::Vector2f playerOnePosition(60.f, 280.f);
    const sf::Vector2f playerTwoPosition(60.f, 680.f);
}

GameOverState::GameOverState(StateStack& stack, Context context)
    : State                 (stack, context),
    m_waitedTime            (0.f),
    m_playerOneScoreText    (std::to_string(context.gameData.playerOne.score), context.gameInstance.getFont("res/fonts/VeraMono.ttf"), 40u),
    m_playerTwoScoreText    (std::to_string(context.gameData.playerTwo.score), context.gameInstance.getFont("res/fonts/VeraMono.ttf"), 40u)
{
    context.renderWindow.setView(context.defaultView);

    backgroundRect.setFillColor({ 0u, 0u, 0u, 168u });
    backgroundRect.setSize(context.defaultView.getSize());

    //context.gameInstance.playMusic(music);

    //parse context data and sum up scores
    auto& tr = context.gameInstance.getTextureResource();

    m_playerOneSprite.setTexture(tr.get("res/textures/playerOne_diffuse.png"));
    //m_playerOneSprite.setNormalMap(tr.get("res/textures/player_normal.png"));
    //m_playerOneSprite.setShader(shader); //using shader needs scene lighting
    m_playerOneSprite.setFrameCount(8u);
    m_playerOneSprite.setFrameRate(maxFrameRate);
    m_playerOneSprite.setFrameSize(frameSize);
    m_playerOneSprite.setLooped(true);
    m_playerOneSprite.play(idle); //TODO switch anim to run while bar goes up
    m_playerOneSprite.setScale(spriteScale, spriteScale);
    m_playerOneSprite.setPosition(playerOnePosition);
    if (!context.gameData.playerOne.enabled) m_playerOneSprite.setColour({ 255u, 255u, 255u, 120u });

    m_playerTwoSprite.setTexture(tr.get("res/textures/playerTwo_diffuse.png"));
    m_playerTwoSprite.setFrameCount(8u);
    m_playerTwoSprite.setFrameRate(maxFrameRate);
    m_playerTwoSprite.setFrameSize(frameSize);
    m_playerTwoSprite.setLooped(true);
    m_playerTwoSprite.play(run);
    m_playerTwoSprite.setScale(spriteScale, spriteScale);
    m_playerTwoSprite.setPosition(playerTwoPosition);
    if (!context.gameData.playerTwo.enabled) m_playerTwoSprite.setColour({ 255u, 255u, 255u, 120u });

    auto& font = context.gameInstance.getFont("res/fonts/VeraMono.ttf");
    m_texts.emplace_back("RESULTS", font, 80u);
    auto& title = m_texts.back();
    Util::Position::centreOrigin(title);
    title.setPosition(context.renderWindow.getView().getCenter());
    title.move(0.f, -500.f);

    m_texts.emplace_back(context.gameData.playerOne.name, font, 40u);
    auto& playerOneName = m_texts.back();
    playerOneName.setPosition(playerOnePosition);
    playerOneName.move(0.f, -80.f);

    m_texts.emplace_back(context.gameData.playerTwo.name, font, 40u);
    auto& playerTwoName = m_texts.back();
    playerTwoName.setPosition(playerTwoPosition);
    playerTwoName.move(0.f, -80.f);

    m_playerOneScoreText.setPosition(playerOnePosition);
    m_playerOneScoreText.move(0.f, 140.f);

    m_playerTwoScoreText.setPosition(playerTwoPosition);
    m_playerTwoScoreText.move(0.f, 140.f);
}

GameOverState::~GameOverState()
{
    //getContext().gameInstance.stopMusic();// playMusic("res/sound/music/intro.ogg");
}

bool GameOverState::update(float dt)
{
    m_waitedTime += dt;

    m_playerOneSprite.update(dt);
    m_playerTwoSprite.update(dt);


    return true; //return true so we can see remaining baddies bouncing about / finish death animations
}

void GameOverState::draw()
{
    if (m_waitedTime > waitTime)
    {
        auto& rw = getContext().renderWindow;
        rw.draw(backgroundRect);
        rw.draw(m_playerOneSprite);
        rw.draw(m_playerTwoSprite);

        for (const auto& t : m_texts)
        {
            rw.draw(t);
        }
        rw.draw(m_playerOneScoreText);
        rw.draw(m_playerTwoScoreText);
    }
}

bool GameOverState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        if (evt.key.code == sf::Keyboard::C)
        {
            
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
    }
    return false;
}