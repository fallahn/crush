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

#include <MenuState.hpp>
#include <Game.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace
{
    sf::Text placeholderText;
}

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
{
    getContext().renderWindow->setTitle("Menu Screen");

    placeholderText.setFont(getContext().gameInstance->getFont());
    placeholderText.setString("Menu goes here.");
}

void MenuState::draw()
{
    getContext().renderWindow->draw(placeholderText);
}

bool MenuState::update(float dt)
{

    return true;
}

bool MenuState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::ID::Game);
    }
    return true;
}