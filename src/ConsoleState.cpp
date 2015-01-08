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

#include <ConsoleState.hpp>
#include <Game.hpp>

ConsoleState::ConsoleState(StateStack& stack, Context context)
    : State     (stack, context),
    m_console   (context.gameInstance.getConsole())
{
    m_console.show();
}

ConsoleState::~ConsoleState()
{
    m_console.show(false);
}

bool ConsoleState::update(float dt)
{
    return false;
}

void ConsoleState::draw()
{
    getContext().renderWindow.draw(m_console);
}

bool ConsoleState::handleEvent(const sf::Event& e)
{
    m_console.handleUIEvent(e);
    return false;
}