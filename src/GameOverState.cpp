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

#include <GameOverState.hpp>
#include <Game.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
    sf::Text placeholderText;
    std::string loseStr =
        "       GAME OVER\n Press C to Continue";

    std::string winStr =
        "        YOU WIN!\n Press C to Continue";

    sf::RectangleShape rectangle;

    const float waitTime = 2.f;

    const std::string music = "res/sound/music/game_over.ogg";
}

GameOverState::GameOverState(StateStack& stack, Context context)
    : State         (stack, context),
    m_waitedTime    (0.f)
{
    context.renderWindow.setTitle("Menu Screen");
    context.renderWindow.setView(context.defaultView);

    placeholderText.setFont(context.gameInstance.getFont("res/fonts/VeraMono.ttf"));
    (!context.gameData.playerOne.enabled && !context.gameData.playerTwo.enabled) ? placeholderText.setString(loseStr) : placeholderText.setString(winStr);
    placeholderText.setCharacterSize(60u);
    Util::Position::centreOrigin(placeholderText);
    placeholderText.setPosition(context.defaultView.getCenter());

    rectangle.setFillColor({ 0u, 0u, 0u, 148u });
    rectangle.setSize(context.defaultView.getSize());

    context.gameInstance.playMusic(music); //TODO make this delayed

    //TODO parse context data and sum up scores
}

bool GameOverState::update(float dt)
{
    m_waitedTime += dt;

    return true; //return true so we can see remaining baddies bouncing about / finish death animations
}

void GameOverState::draw()
{
    if (m_waitedTime > waitTime)
    {
        getContext().renderWindow.draw(rectangle);
        getContext().renderWindow.draw(placeholderText);
    }
}

bool GameOverState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        if (evt.key.code == sf::Keyboard::C)
        {
            
            requestStackClear();
            auto& gameData = getContext().gameData;
            if (++gameData.mapIndex == gameData.mapList.size() //reached the end of the map list
                || (!gameData.playerOne.enabled && !gameData.playerTwo.enabled)) //both players are dead
            {
                //go back to main menu
                requestStackPush(States::ID::Menu);
            }
            else
            {
                //load next map
                requestStackPush(States::ID::Game);
            }
        }
    }
    return false;
}