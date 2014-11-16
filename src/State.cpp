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

#include <State.hpp>
#include <StateStack.hpp>

State::Context::Context(sf::RenderWindow& window, Game& game)
    : renderWindow(&window), gameInstance(&game){}

State::State(StateStack& stateStack, Context context)
    : m_stack(&stateStack),
    m_context(context){}

void State::requestStackPush(States::ID id)
{
    m_stack->pushState(id);
}

void State::requestStackPop()
{
    m_stack->popState();
}

void State::requestStackClear()
{
    m_stack->clearStates();
}

State::Context State::getContext() const
{
    return m_context;
}