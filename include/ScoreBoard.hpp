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

//observes the game to update the scores

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <Observer.hpp>
#include <State.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>

class ScoreBoard final : public Observer, public Subject, public sf::Drawable, private sf::NonCopyable
{
public:
    ScoreBoard(StateStack& stack, State::Context context);
    ~ScoreBoard() = default;

    void onNotify(Subject& s, const game::Event& evt) override;

    void enablePlayer(Category::Type player);

private:
    StateStack& m_stack;
    State::Context m_context;

    sf::Int16 m_playerOneLives;
    sf::Int16 m_playerTwoLives;

    sf::Uint16 m_playerOneScore;
    sf::Uint16 m_playerTwoScore;

    bool m_playerOneExtinct;
    bool m_playerTwoExtinct;

    void updateText(Category::Type type);

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //HUD_H_