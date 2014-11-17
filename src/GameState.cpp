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

#include <GameState.hpp>
#include <Game.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace
{
    Camera sceneCam;
    sf::CircleShape circle(25.f);
}

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context)
{
    getContext().renderWindow->setTitle("Game Screen");
    
    auto camNode = std::make_unique<Node>("camNode");
    sceneCam.setView({ {}, { 1280.f, 960.f } });
    camNode->setCamera(&sceneCam);
    camNode->setPosition(sceneCam.getView().getSize() / 2.f);
    m_scene.addNode(camNode);

    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(3.f);
    circle.setOrigin(25.f, 25.f);

    auto circleNode = std::make_unique<Node>("circleNode");
    circleNode->setPosition(400.f, 400.f);
    circleNode->setDrawable(&circle);
    m_scene.addNode(circleNode);

}

void GameState::draw()
{
    getContext().renderWindow->draw(m_scene);
}

bool GameState::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_scene.findNode("camNode")->move(80.f * dt, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        m_scene.findNode("circleNode")->move(0.f, 82.f * dt);

    return true;
}

bool GameState::handleEvent(const sf::Event& evt)
{
    
    
    return true;
}