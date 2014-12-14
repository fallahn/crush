/*********************************************************************
Matt Marchant 2014
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

    const sf::Uint16 npcPoints = 50; //points for killing npc
    const sf::Uint16 playerPoints = 100; //points for killing other player
    const sf::Uint16 crushPoints = 500; //points for crushing someone
    const sf::Uint16 suicidePoints = 200; //points deducted for accidentally crushing self
}

ScoreBoard::ScoreBoard(StateStack& stack, State::Context context)
    : m_stack           (stack),
    m_context           (context),
    m_playerOneLives    (5),
    m_playerTwoLives    (-1),
    m_playerOneScore    (0u),
    m_playerTwoScore    (0u),
    m_playerOneExtinct  (false),
    m_playerTwoExtinct  (false),
    m_maxNpcs           (2u),
    m_spawnedNpcs       (0u),
    m_deadNpcs          (0u)
{
    playerOneText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    playerOneText.setPosition({ 60.f, 10.f });
    updateText(Category::PlayerOne);

    playerTwoText.setFont(*playerOneText.getFont());
    playerTwoText.setString("Press Start");
    playerTwoText.setPosition({ 1400.f, 10.f });

    npcText.setFont(*playerOneText.getFont());
}

//public
void ScoreBoard::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Node:
        if (evt.node.action == game::Event::NodeEvent::Despawn)
        {
            switch (evt.node.type)
            {
            case Category::PlayerOne:
                m_playerOneLives--;
                
                if (m_playerOneLives < 0)
                {
                    disablePlayer(Category::PlayerOne);
                }

                updateText(evt.node.type);
                break;
            case Category::PlayerTwo:
                m_playerTwoLives--;
                
                if (m_playerTwoLives < 0)
                {
                    disablePlayer(Category::PlayerTwo);
                }
                updateText(evt.node.type);
                break;
            case Category::Npc:
                m_deadNpcs++;
                if (m_maxNpcs == m_deadNpcs)
                {
                    //game over, all dead
                    m_stack.pushState(States::ID::GameOver);

                    //disable player input
                    disablePlayer(Category::PlayerOne);
                    disablePlayer(Category::PlayerTwo);
                }
                
                break;
            default: break;
            }
        }
        else if (evt.node.action == game::Event::NodeEvent::Spawn)
        {
            switch (evt.node.type)
            {
            case Category::Npc:
                m_spawnedNpcs++;
                updateText(Category::Npc);

                if (m_spawnedNpcs == m_maxNpcs)
                {
                    //stop spawning
                    game::Event e;
                    e.type = game::Event::Game;
                    e.game.action = game::Event::GameEvent::NpcDisable;
                    notify(*this, e);
                }
                
                break;
            default: break;
            }
        }
        else if(evt.node.action == game::Event::NodeEvent::KilledNode)
        {
            auto textUpdateTarget = evt.node.type;
            switch (evt.node.type)
            {
            case Category::PlayerOne:
                if (evt.node.target == Category::Npc)
                {
                    m_playerOneScore += npcPoints;
                }
                else if (evt.node.target == Category::PlayerTwo)
                {
                    m_playerOneScore += playerPoints;
                }
                break;
            case Category::PlayerTwo:
                if (evt.node.target == Category::Npc)
                {
                    m_playerTwoScore += npcPoints;
                }
                else if (evt.node.target == Category::PlayerOne)
                {
                    m_playerTwoScore += playerPoints;
                }
                break;
            case Category::Block:
                if (evt.node.owner == Category::PlayerOne)
                {
                        switch (evt.node.target)
                        {
                        case Category::PlayerTwo: //p1 crushed p2
                            m_playerOneLives++;
                        case Category::Npc: //p1 killed bad guy
                            m_playerOneScore += crushPoints;
                            break;
                        case Category::PlayerOne: //p1 killed self, doh
                            (m_playerOneScore > suicidePoints) ?
                                m_playerOneScore -= suicidePoints :
                                m_playerOneScore = 0u;
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
                        case Category::Npc: //p2 killed bad guy
                            m_playerTwoScore += crushPoints;
                            break;
                        case Category::PlayerTwo: //p2 crushed self :S
                            (m_playerTwoScore > suicidePoints) ?
                                m_playerTwoScore -= suicidePoints :
                                m_playerTwoScore = 0u;
                            break;
                        default: break;
                        }
                }
                else if (evt.node.owner == Category::None)
                {
                    switch (evt.node.target)
                    {
                    case Category::PlayerOne: //p1 killed self, doh
                        (m_playerOneScore > suicidePoints) ?
                            m_playerOneScore -= suicidePoints :
                            m_playerOneScore = 0u;
                        break;
                    case Category::PlayerTwo: //p2 crushed self :S
                        (m_playerTwoScore > suicidePoints) ?
                            m_playerTwoScore -= suicidePoints :
                            m_playerTwoScore = 0u;
                        break;
                    default: break;
                    }
                }
                textUpdateTarget = evt.node.owner;
                break;
            default: break;
            }
            if (textUpdateTarget != Category::None)
                updateText(textUpdateTarget);
        }
        break;
    case game::Event::Player:

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

    game::Event e;
    e.type = game::Event::Game;
    e.game.action = (player == Category::PlayerOne) ? 
        game::Event::GameEvent::PlayerOneEnable : 
        game::Event::GameEvent::PlayerTwoEnable;

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
        npcText.setPosition(m_context.defaultView.getCenter() + sf::Vector2f(0.f, 525.f));
    }

    if (m_playerOneLives < 0 && m_playerTwoLives < 0)
    {
        //Gaaaaaaame Oveeeeer!!!
        m_stack.pushState(States::ID::GameOver);
    }
}

void ScoreBoard::disablePlayer(Category::Type player)
{
    assert(player == Category::PlayerOne || player == Category::PlayerTwo);

    game::Event e;
    e.type = game::Event::Game;
    e.game.action = (player == Category::PlayerOne) ? 
        game::Event::GameEvent::PlayerOneDisable :
        game::Event::GameEvent::PlayerTwoDisable;
    notify(*this, e);

    if (player == Category::PlayerOne)
        m_playerOneExtinct = true;
    else
        m_playerTwoExtinct = true;
}

void ScoreBoard::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    m_context.renderWindow.draw(playerOneText);
    m_context.renderWindow.draw(playerTwoText);
    m_context.renderWindow.draw(npcText);
}