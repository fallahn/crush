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

#include <TitleState.hpp>
#include <Game.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Resource.hpp>
#include <Util.hpp>

#include <FileSystem.hpp>
#include <iostream>

namespace
{
    sf::Text titleText;
    sf::Text bigText;

    const std::string music = "res/sound/music/intro.ogg";
}

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context)
{
    context.gameData.mapList = FileSystem::listFiles("res/maps");

    //TODO remove this after testing on linux
    std::cout << "found " << context.gameData.mapList.size() << " files:" << std::endl;
    for (const auto& s : context.gameData.mapList)
        std::cout << s << std::endl;
    
    context.renderWindow.setTitle("Title Screen");
    context.renderWindow.setView(context.defaultView);

    titleText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    titleText.setCharacterSize(36u);
    titleText.setString("Press any key to continue...");

    Util::Position::centreOrigin(titleText);
    titleText.setPosition(context.defaultView.getCenter());

    bigText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    bigText.setCharacterSize(100u);
    bigText.setString("CRUSH!");
    Util::Position::centreOrigin(bigText);
    bigText.setPosition(titleText.getPosition() - sf::Vector2f(0.f, bigText.getLocalBounds().height + 20.f));

    context.gameInstance.playMusic(music);
}

void TitleState::draw()
{
    getContext().renderWindow.draw(titleText);
    getContext().renderWindow.draw(bigText);
}

bool TitleState::update(float dt)
{

    return true;
}

bool TitleState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::ID::Menu);
    }
    return true;
}