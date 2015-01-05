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
        "Use WAD to move Player One and Arrow keys\n"
        "to move Player Two. Hold Space (Player One)\n"
        "or Right Shift (Player Two) to grab a box and\n"
        "drag it around. Press S (Player One) or Down\n"
        "(Player Two) to pick up a block and stack it\n"
        "on others (also works with a controller!).\n"
        "Score the most points by crushing the bad guys\n"
        "(or each other!). Crush enemies between blocks..\n"
        " but don't accidentally crush yourself!\n"
        "\n"
        "Press Enter to begin.";

    const std::string music = "res/sound/music/main_menu.ogg";
}

MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
{
    context.renderWindow.setTitle("Menu Screen");
    context.renderWindow.setView(context.defaultView);

    placeholderText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    placeholderText.setString(str);
    placeholderText.setCharacterSize(58u);
    Util::Position::centreOrigin(placeholderText);
    placeholderText.setPosition(context.defaultView.getCenter());


    //context.gameInstance.playMusic(music);

    //reset game data TODO make sure to load correct keybinds
    //and enable correct amount of players
    context.gameData.playerOne = {};
    //context.gameData.playerOne.enabled = false;
    context.gameData.playerTwo = {};
    context.gameData.playerTwo.enabled = false;
    context.gameData.mapIndex = 0;
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