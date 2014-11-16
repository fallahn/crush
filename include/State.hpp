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

//ABC for game states

#ifndef STATE_H_
#define STATE_H_

#include <StateIds.hpp>

#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
    class RenderWindow;
}

class StateStack;
class Game;
class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(sf::RenderWindow& renderWindow, Game& game);
        sf::RenderWindow* renderWindow;
        Game* gameInstance;
    };

    State(StateStack& stateStack, Context context);
    virtual ~State() = default;

    virtual void draw() = 0;
    virtual bool update(float dt) = 0;
    virtual bool handleEvent(const sf::Event& evt) = 0;

protected:
    void requestStackPush(States::ID id);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

private:

    StateStack* m_stack;
    Context m_context;

};

#endif //STATE_H_