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

#ifndef GAME_OVER_STATE_H_
#define GAME_OVER_STATE_H_

#include <State.hpp>
#include <AnimatedIcon.hpp>
#include <AnimatedSprite.hpp>
#include <ScoreBar.hpp>
#include <TimedEvent.hpp>

#include <SFML/Graphics/Text.hpp>

#include <list>
#include <vector>

class GameOverState final : public State
{
public:
    GameOverState(StateStack& stack, Context context);
    ~GameOverState();// = default;
    
    bool handleEvent(const sf::Event& evt) override;
    bool update(float dt) override;
    void draw() override;

private:

    AnimatedSprite m_playerOneSprite;
    AnimatedSprite m_playerTwoSprite;

    std::vector<sf::Text> m_texts;
    sf::Text m_playerOneScoreText;
    sf::Text m_playerTwoScoreText;
    sf::Text m_statusText;
    float m_playerOneRunningScore;
    float m_playerTwoRunningScore;

    std::vector<ScoreBar> m_playerOneBar;
    std::vector<ScoreBar> m_playerTwoBar;
    sf::Uint8 m_barIndex;

    AnimatedIcon m_hatIcon;

    std::list<TimedEvent> m_timedEvents;
    bool m_drawingEnabled;
    bool m_animateBars;
    bool m_doHatBonus;
    bool m_canContinue;

    void initSprite(AnimatedSprite&, const sf::Vector2f&);
    void continueGame();
    void animateBar();
};

#endif // GAME_OVER_STATE_H_