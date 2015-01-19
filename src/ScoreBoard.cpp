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

#include <ScoreBoard.hpp>
#include <Game.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <sstream>
#include <cassert>
#include <iostream>

namespace
{
    sf::Text playerOneText;
    sf::Text playerTwoText;
    sf::Text npcText;
    sf::Text timerText;

    sf::Font messageFont;

    const sf::Uint16 playerPoints = 100u; //points for killing other player
    const sf::Uint16 crushPoints = 500u; //points for crushing someone
    const sf::Uint16 suicidePoints = 200u; //points deducted for accidentally crushing self
    const sf::Uint16 itemPoints = 400u; //points for collecting item
    const sf::Uint16 killStreakPoints = 500u;
    const sf::Int8 killStreakStep = 5; //number of kills in a streak

    const float messageAcceleration = 232.f;
    const float initialMessageSpeed = 60.f;
}

ScoreBoard::ScoreBoard(StateStack& stack, State::Context context)
    : m_stack               (stack),
    m_context               (context),
    m_playerOneLives        (5),
    m_playerTwoLives        (-1),
    m_playerOneScore        (0u),
    m_playerTwoScore        (0u),
    m_playerOneKillStreak   (0u),
    m_playerTwoKillStreak   (0u),
    m_playerOneHatTime      (&context.gameData.playerOne.hatTime),
    m_playerTwoHatTime      (&context.gameData.playerTwo.hatTime),
    m_nullHatTime           (0.f),
    m_hatTimer              (&m_nullHatTime),
    m_playerOneExtinct      (false),
    m_playerTwoExtinct      (false),
    m_maxNpcs               (2u),
    m_spawnedNpcs           (0u),
    m_deadNpcs              (0u)
{
    playerOneText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    playerOneText.setPosition(60.f, 10.f );
    //updateText(Category::PlayerOne);

    playerTwoText.setFont(*playerOneText.getFont());
    //playerTwoText.setString("Press Start");
    playerTwoText.setPosition(1400.f, 10.f);

    npcText.setFont(*playerOneText.getFont());

    //initialise player data from current context
    m_playerOneExtinct = !context.gameData.playerOne.enabled;
    m_playerOneLives = (m_playerOneExtinct) ? -1 : context.gameData.playerOne.lives;
    m_playerOneScore = context.gameData.playerOne.score;
    updateText(Category::PlayerOne);

    m_playerTwoExtinct = !context.gameData.playerTwo.enabled;
    m_playerTwoLives = (m_playerTwoExtinct) ? -1 : context.gameData.playerTwo.lives;
    m_playerTwoScore = context.gameData.playerTwo.score;
    updateText(Category::PlayerTwo);

    messageFont = context.gameInstance.getFont("res/fonts/VeraMono.ttf");

    timerText.setFont(messageFont);
    timerText.setPosition(60.f, 50.f);
    *m_playerOneHatTime = 0.f;
    *m_playerTwoHatTime = 0.f;
}

//public
void ScoreBoard::update(float dt)
{
    m_messages.remove_if([](const Message& m){return m.stopped(); });

    for (auto& m : m_messages)
        m.update(dt);

    //update current timer
    *m_hatTimer += dt;

    timerText.setString(std::to_string(static_cast<sf::Uint16>(m_nullHatTime)) + ", " 
        + std::to_string(static_cast<sf::Uint16>(*m_playerOneHatTime)) + ", " 
        + std::to_string(static_cast<sf::Uint16>(*m_playerTwoHatTime)) + ", "
        + std::to_string(m_playerOneKillStreak) + ", "
        + std::to_string(m_playerTwoKillStreak));
}

void ScoreBoard::onNotify(Subject& s, const Event& evt)
{
    switch (evt.type)
    {
    case Event::Node:
        if (evt.node.action == Event::NodeEvent::Despawn)
        {
            switch (evt.node.type)
            {
            case Category::PlayerOne:
                m_playerOneLives--;
                
                if (m_playerOneLives < 0)
                {
                    disablePlayer(Category::PlayerOne);
                }
                m_playerOneKillStreak = 0u;
                updateText(evt.node.type);
                break;
            case Category::PlayerTwo:
                m_playerTwoLives--;
                
                if (m_playerTwoLives < 0)
                {
                    disablePlayer(Category::PlayerTwo);
                }
                updateText(evt.node.type);
                m_playerTwoKillStreak = 0u;
                break;
            case Category::Npc:
                m_deadNpcs++;
                if (m_maxNpcs == m_deadNpcs)
                {
                    //game over, all dead
                    m_stack.pushState(States::ID::GameOver);
                    updateGameData();

                    //disable player input
                    disablePlayer(Category::PlayerOne);
                    disablePlayer(Category::PlayerTwo);
                }
                
                break;
            default: break;
            }
        }
        else if (evt.node.action == Event::NodeEvent::Spawn)
        {
            switch (evt.node.type)
            {
            case Category::Npc:
                m_spawnedNpcs++;
                updateText(Category::Npc);

                if (m_spawnedNpcs == m_maxNpcs)
                {
                    //stop spawning
                    Event e;
                    e.type = Event::Game;
                    e.game.action = Event::GameEvent::NpcDisable;
                    notify(*this, e);
                }
                
                break;
            default: break;
            }
        }
        else if(evt.node.action == Event::NodeEvent::KilledNode)
        {
            auto textUpdateTarget = evt.node.type;
            switch (evt.node.type)
            {                
            case Category::Block:
                if (evt.node.owner == Category::PlayerOne)
                {
                        switch (evt.node.target)
                        {
                        case Category::PlayerTwo: //p1 crushed p2
                            m_playerOneLives++;
                            m_playerOneScore += playerPoints;

                            //show message
                            m_messages.emplace_back(std::to_string(playerPoints),
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);


                        case Category::Npc: //p1 killed bad guy
                            m_playerOneScore += crushPoints;

                            //show message
                            m_messages.emplace_back(std::to_string(crushPoints),
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);

                            m_playerOneKillStreak++;
                            if ((m_playerOneKillStreak % killStreakStep) == 0)
                            {
                                killstreakMessage();
                                m_playerOneScore += (m_playerOneKillStreak / killStreakStep) * killStreakPoints;
                            }

                            break;
                        case Category::PlayerOne: //p1 killed self, doh
                        {
                            std::string msg = "-";
                            if (m_playerOneScore > suicidePoints)
                            {
                                m_playerOneScore -= suicidePoints;
                                msg += std::to_string(suicidePoints);
                            }
                            else
                            {
                                msg += std::to_string(m_playerOneScore);
                                m_playerOneScore = 0u;
                            }

                            m_messages.emplace_back(msg,
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);
                        }
                            break;
                        default: break;
                        }
                }
                else if (evt.node.owner == Category::PlayerTwo)
                {
                        switch (evt.node.target)
                        {
                        case Category::PlayerOne: //p2 killed p1  
                            m_playerTwoLives++;
                            m_playerTwoScore += playerPoints;

                            //show message
                            m_messages.emplace_back(std::to_string(playerPoints),
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);

                        case Category::Npc: //p2 killed bad guy
                            m_playerTwoScore += crushPoints;

                            //show message
                            m_messages.emplace_back(std::to_string(crushPoints),
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);

                            m_playerTwoKillStreak++;
                            if (m_playerTwoKillStreak % killStreakStep == 0)
                            {
                                m_playerTwoScore += (m_playerTwoKillStreak / killStreakStep) * killStreakPoints;
                                killstreakMessage();
                            }
                            break;
                        case Category::PlayerTwo: //p2 crushed self :S
                        {    
                            std::string msg = "-";
                            if (m_playerTwoScore > suicidePoints)
                            {
                                m_playerTwoScore -= suicidePoints;
                                msg += std::to_string(suicidePoints);
                            }
                            else
                            {
                                msg += std::to_string(m_playerTwoScore);
                                m_playerTwoScore = 0u;
                            }

                            m_messages.emplace_back(msg,
                                sf::Vector2f(evt.node.positionX, evt.node.positionY),
                                messageFont);
                        }
                            break;
                        default: break;
                        }
                }
                else if (evt.node.owner == Category::None)
                {
                    std::string msg = "-";
                    switch (evt.node.target)
                    {
                    case Category::PlayerOne: //p1 killed self, doh
                        if (m_playerOneScore > suicidePoints)
                        {
                            msg += std::to_string(suicidePoints);
                            m_playerOneScore -= suicidePoints;
                        }
                        else
                        {
                            msg += std::to_string(m_playerOneScore);
                            m_playerOneScore = 0u;
                        }
                        break;
                    case Category::PlayerTwo: //p2 crushed self :S
                        if (m_playerTwoScore > suicidePoints)
                        {
                            msg += std::to_string(suicidePoints);
                            m_playerTwoScore -= suicidePoints;
                        }
                        else
                        {
                            msg += std::to_string(m_playerTwoScore);
                            m_playerTwoScore = 0u;
                        }
                        break;
                    case Category::Npc:
                        //NPC killed self so shouldn't be subtracted
                        m_deadNpcs--; //TODO if this gets called before Despawn increases
                        m_spawnedNpcs--;
                        //then we run the risk of flipping over to 255? :S
                        break;
                    case Category::HatCarried:
                    case Category::HatDropped:
                        msg = "POF!";
                        m_hatTimer = &m_nullHatTime;
                        m_context.gameData.playerOne.hasHat = false;
                        m_context.gameData.playerTwo.hasHat = false;
                        break;
                    default: break;
                    }

                    m_messages.emplace_back(msg,
                        sf::Vector2f(evt.node.positionX, evt.node.positionY),
                        messageFont);
                }
                textUpdateTarget = evt.node.owner;
                break;

            case Category::Npc:
                if (evt.node.target == Category::HatDropped)
                {
                    m_hatTimer = &m_nullHatTime;
                    m_context.gameData.playerOne.hasHat = false;
                    m_context.gameData.playerTwo.hasHat = false;

                    m_messages.emplace_back("PAF!",
                        sf::Vector2f(evt.node.positionX, evt.node.positionY),
                        messageFont);
                }
                break;
            default: break;
            }
            //if (textUpdateTarget != Category::None)
            updateText(textUpdateTarget);
        }
        break;
    case Event::Player:
    {
        std::string msg;
        switch (evt.player.action)
        {
        case Event::PlayerEvent::GotItem:
            switch (evt.player.item)
            {
            case Event::PlayerEvent::ExtraLife:
                (evt.player.playerId == Category::PlayerOne) ?
                    m_playerOneLives++ :
                    m_playerTwoLives++;
                msg = "Extra Life! ";
                break;
            case Event::PlayerEvent::Attraction:
                msg = "Nothing! ";
                break;
            case Event::PlayerEvent::ExtraSpeed:
                msg = "Extra Speed! ";
                break;
            case Event::PlayerEvent::JumpIncrease:
                msg = "Extra Jump! ";
                break;
            case Event::PlayerEvent::ReverseControls:
                msg = "Reverse Controls! ";
                break;
            default: break;
            }
            (evt.player.playerId == Category::PlayerOne) ?
                m_playerOneScore += itemPoints :
                m_playerTwoScore += itemPoints;

            //display message
            m_messages.emplace_back(msg + std::to_string(itemPoints),
                sf::Vector2f(evt.player.positionX, evt.player.positionY),
                messageFont);

            m_messages.back().setColour(sf::Color::Yellow);
            updateText(evt.player.playerId);
            break;
        case Event::PlayerEvent::GotHat:
            if (evt.player.playerId == Category::PlayerOne)
            {
                m_hatTimer = m_playerOneHatTime;
                m_context.gameData.playerOne.hasHat = true;
            }
            else
            {
                m_hatTimer = m_playerTwoHatTime;
                m_context.gameData.playerTwo.hasHat = true;
            }
            break;
        case Event::PlayerEvent::LostHat:
            m_hatTimer = &m_nullHatTime;
            m_context.gameData.playerOne.hasHat = false;
            m_context.gameData.playerTwo.hasHat = false;
            break;

        default:break;
        }
    }
        break;
    default: break;
    }
}

void ScoreBoard::enablePlayer(Category::Type player)
{
    assert(player == Category::PlayerOne || player == Category::PlayerTwo);

    //prevent re-enabling players who have lost all lives
    if ((player == Category::PlayerOne && m_playerOneExtinct)
        || (player == Category::PlayerTwo && m_playerTwoExtinct))
    {
        return;
    }

    Event e;
    e.type = Event::Game;
    e.game.action = (player == Category::PlayerOne) ? 
        Event::GameEvent::PlayerOneEnable : 
        Event::GameEvent::PlayerTwoEnable;

    notify(*this, e);

    if (player == Category::PlayerTwo)
    {
        m_playerTwoLives = 5;
        updateText(Category::PlayerTwo);
    }
}

void ScoreBoard::setMaxNpcs(sf::Uint8 count)
{
    m_maxNpcs = count;
    updateText(Category::Npc);
}

//private
void ScoreBoard::updateText(Category::Type type)
{
    std::stringstream ss;    
    if (type == Category::PlayerOne)
    {
        if (m_playerOneLives >= 0)
        {
            ss << "Lives: " << m_playerOneLives << "    Score: " << m_playerOneScore;
            playerOneText.setString(ss.str());
        }
        else if (m_playerOneExtinct)
        {
            ss << "GAME OVER    Score: " << m_playerOneScore;
            playerOneText.setString(ss.str());
        }
        
    }
    else if (type == Category::PlayerTwo)
    {
        if (m_playerTwoLives >= 0)
        {
            ss << "Lives: " << m_playerTwoLives << "    Score: " << m_playerTwoScore;
            playerTwoText.setString(ss.str());
        }
        else if (m_playerTwoExtinct)
        {
            ss << "GAME OVER    Score: " << m_playerTwoScore;
            playerTwoText.setString(ss.str());
        }
    }
    else if (type == Category::Npc)
    {
        ss << "Enemies Remaining: " << (m_maxNpcs - m_spawnedNpcs) << std::endl;
        npcText.setString(ss.str());
        Util::Position::centreOrigin(npcText);
        npcText.setPosition(m_context.defaultView.getCenter() + sf::Vector2f(0.f, -500.f));
    }

    //not really sure why this is in this function as it's not the obvious place to look
    if (m_playerOneLives < 0 && m_playerTwoLives < 0)
    {
        //Gaaaaaaame Oveeeeer!!!
        m_stack.pushState(States::ID::GameOver);
        updateGameData();
    }
}

void ScoreBoard::updateGameData()
{
    auto& gameData = m_context.gameData;
    gameData.playerOne.enabled = !m_playerOneExtinct;
    gameData.playerOne.lives = m_playerOneLives;
    gameData.playerOne.score = m_playerOneScore;

    gameData.playerTwo.enabled = !m_playerTwoExtinct;
    gameData.playerTwo.lives = m_playerTwoLives;
    gameData.playerTwo.score = m_playerTwoScore;
}

void ScoreBoard::disablePlayer(Category::Type player)
{
    assert(player == Category::PlayerOne || player == Category::PlayerTwo);

    Event e;
    e.type = Event::Game;
    e.game.action = (player == Category::PlayerOne) ? 
        Event::GameEvent::PlayerOneDisable :
        Event::GameEvent::PlayerTwoDisable;
    notify(*this, e);

    if (player == Category::PlayerOne)
        m_playerOneExtinct = true;
    else
        m_playerTwoExtinct = true;
}

void ScoreBoard::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    for (const auto& m : m_messages)
        m_context.renderWindow.draw(m);

    m_context.renderWindow.draw(playerOneText);
    m_context.renderWindow.draw(playerTwoText);
    m_context.renderWindow.draw(npcText);

    m_context.renderWindow.draw(timerText);
}

void ScoreBoard::killstreakMessage()
{
    m_messages.emplace_back("CRUSHTASTIC!",
        m_context.renderWindow.getView().getCenter(),
        messageFont, true);
}

//-----message class-----//
ScoreBoard::Message::Message(const std::string& text, const sf::Vector2f& position, const sf::Font& font, bool zoom)
    : m_colour          (sf::Color::White),
    m_transparency      (1.f),
    m_zoom              (zoom),
    m_text              (text, font, 24u),
    m_messageSpeed      (initialMessageSpeed)
{
    Util::Position::centreOrigin(m_text);
    m_text.setPosition(position);

    m_text.setStyle(sf::Text::Bold);
}

void ScoreBoard::Message::update(float dt)
{
    const float step = 1.4f * dt;
    m_transparency -= step;

    if (m_transparency > 0)
    {
        m_colour.a = static_cast<sf::Uint8>(255.f * m_transparency);
        m_text.setColor(m_colour);
    }

    if (m_zoom)
    {
        const float amount = 1.04f + dt;
        m_text.scale(amount, amount);
    }
    else
    {
        m_messageSpeed += messageAcceleration * dt;
        m_text.move(0.f, -m_messageSpeed * dt);
    }
}

void ScoreBoard::Message::setColour(const sf::Color& c)
{
    m_colour = c;
}

bool ScoreBoard::Message::stopped() const
{
    return (m_transparency <= 0);
}

void ScoreBoard::Message::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.draw(m_text, states);
}