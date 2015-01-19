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
#include <UIButton.hpp>

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
    context.renderWindow.setView(context.defaultView);

    //reset game data TODO make sure to load correct keybinds and names
    context.gameData.playerOne = {};
    context.gameData.playerOne.name = "Sidney Burnsides";
    context.gameData.playerTwo = {};
    context.gameData.playerTwo.name = "Gemima Headson";
    context.gameData.mapIndex = 0;

    auto& font = context.gameInstance.getFont("res/fonts/VeraMono.ttf");
    auto& tr = context.gameInstance.getTextureResource();
    //player one button
    auto playerOneButton = std::make_shared<ui::Button>(font, tr.get("res/textures/ui/button.png"));
    playerOneButton->setText("One Player");
    playerOneButton->setPosition(960.f, 200.f);
    playerOneButton->setTextColour(sf::Color::Black);
    playerOneButton->setCallback([this]()
    {
        getContext().gameData.playerTwo.enabled = false;
        requestStackPop();
        requestStackPush(States::ID::Game);
    });
    m_uiContainer.addControl(playerOneButton);

    //player two button
    auto playerTwoButton = std::make_shared<ui::Button>(font, tr.get("res/textures/ui/button.png"));
    playerTwoButton->setText("Two Player");
    playerTwoButton->setPosition(960.f, 248.f);
    playerTwoButton->setTextColour(sf::Color::Black);
    playerTwoButton->setCallback([this]()
    {
        requestStackPop();
        requestStackPush(States::ID::Game);
    });
    m_uiContainer.addControl(playerTwoButton);

    //options button
    auto optionsButton = std::make_shared<ui::Button>(font, tr.get("res/textures/ui/button.png"));
    optionsButton->setText("Options");
    optionsButton->setTextColour(sf::Color::Black);
    optionsButton->setPosition(960.f, 296.f);
    //TODO set callback to push options state
    m_uiContainer.addControl(optionsButton);

    //help button
    auto helpButton = std::make_shared<ui::Button>(font, tr.get("res/textures/ui/button.png"));
    helpButton->setText("How To Play");
    helpButton->setTextColour(sf::Color::Black);
    helpButton->setPosition(960.f, 344.f);
    //TODO set callback to display help text
    m_uiContainer.addControl(helpButton);

    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UIMove, "res/sound/ui/move.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UISelect, "res/sound/ui/select.wav");
}

void MenuState::draw()
{
    getContext().renderWindow.draw(m_uiContainer);
}

bool MenuState::update(float dt)
{

    return true;
}

bool MenuState::handleEvent(const sf::Event& evt)
{
    m_uiContainer.handleEvent(evt);
    return true;
}