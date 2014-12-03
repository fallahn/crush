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
#include <Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace
{
    sf::Text placeholderText;

    std::string str =
        "HOW TO PLAY:\n"
        "\n"
        "Use WAD to move player one and Arrow keys\n"
        "to move player two. Press 2 to spawn the second\n"
        "player. Hold Space (player one) or Right Shift\n"
        "(player two) to grab a box and drag it around.\n"
        "Score the most points by crushing the bad guys,\n"
        "(or each other!). Crush enemies between blocks, or\n"
        "by jumping on them - but don't acidentally crush\n"
        "yourself!\n"
        "\n"
        "Press Enter to begin.";
}

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
{
    getContext().renderWindow.setTitle("Menu Screen");
    getContext().renderWindow.setView(getContext().defaultView);

    placeholderText.setFont(getContext().gameInstance.getFont("default"));
    placeholderText.setString(str);
    placeholderText.setCharacterSize(60u);
    Util::Position::centreOrigin(placeholderText);
    placeholderText.setPosition(getContext().defaultView.getCenter());
    
}

void MenuState::draw()
{
    getContext().renderWindow.draw(placeholderText);
}

bool MenuState::update(float dt)
{

    return true;
}

bool MenuState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        if (evt.key.code == sf::Keyboard::Return)
        {
            requestStackPop();
            requestStackPush(States::ID::Game);
        }
    }
    return true;
}