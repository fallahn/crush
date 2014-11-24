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
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
    Camera sceneCam;
    sf::RectangleShape rectangleShape({ 130.f, 130.f });
    sf::RectangleShape groundShape;
}

GameState::GameState(StateStack& stack, Context context)
    : State     (stack, context),
    m_physWorld (9.f)
{
    getContext().renderWindow->setTitle("Game Screen");
    
    auto camNode = std::make_unique<Node>("camNode");
    sceneCam.setView({ {}, { 1280.f, 960.f } });
    camNode->setCamera(&sceneCam);
    camNode->setPosition(sceneCam.getView().getSize() / 2.f);
    m_scene.addNode(camNode);

    rectangleShape.setFillColor(sf::Color::Transparent);
    rectangleShape.setOutlineColor(sf::Color::Red);
    rectangleShape.setOutlineThickness(3.f);
    groundShape = rectangleShape;

    auto rectangleNode = std::make_unique<Node>("rectangleNode");
    rectangleNode->setPosition(400.f, 400.f);
    rectangleNode->setDrawable(&rectangleShape);

    PhysWorld::PhysData pd(1.f, 0.5f);
    auto physObj = m_physWorld.addObject({ {}, { 130.f, 130.f } }, pd);
    rectangleNode->setPhysObject(physObj);
    m_scene.addNode(rectangleNode);

    groundShape.setSize({ 1280.f, 200.f });
    auto groundNode = std::make_unique<Node>("groundNode");
    groundNode->setDrawable(&groundShape);
    groundNode->setPosition(0.f, 760.f);

    pd.mass = pd.inverseMass = 0.f;
    auto groundObj = m_physWorld.addObject({ {}, { 1280.f, 200.f } }, pd);
    groundNode->setPhysObject(groundObj);
    m_scene.addNode(groundNode);
}

bool GameState::update(float dt)
{
    m_physWorld.step(dt);
    return true;
}

void GameState::draw()
{
    getContext().renderWindow->draw(m_scene);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    
    
    return true;
}