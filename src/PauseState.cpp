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

#include <PauseState.hpp>
#include <Game.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics//Text.hpp>

namespace
{
    sf::RectangleShape grey;
    sf::Text text;
}

PauseState::PauseState(StateStack& stack, Context context)
    : State(stack, context)
{
    context.renderWindow.setView(context.defaultView);
    
    grey.setFillColor({ 0u, 0u, 0u, 148u });
    grey.setSize(context.defaultView.getSize());

    text.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    text.setString("PAUSED");
    text.setCharacterSize(80u);
    Util::Position::centreOrigin(text);
    text.setPosition(context.defaultView.getCenter());

    context.gameInstance.pauseMusic();
}

PauseState::~PauseState()
{
    getContext().gameInstance.resumeMusic();
}

bool PauseState::update(float dt)
{
    return false;
}

void PauseState::draw()
{
    getContext().renderWindow.draw(grey);
    getContext().renderWindow.draw(text);
}

bool PauseState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::P:
            requestStackPop();
            break;
        default: break;
        }
    }
    else if (evt.type == sf::Event::JoystickButtonPressed)
    {
        if (evt.joystickButton.button == 7)
            requestStackPop();
    }
    return false;
}

