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

    CollisionWorld::Body* body;
}

GameState::GameState(StateStack& stack, Context context)
    : State             (stack, context),
    m_collisionWorld    (10.f)
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
    body = m_collisionWorld.addBody(CollisionWorld::Body::Type::Block, { {}, { 130.f, 130.f } });
    rectangleNode->setCollisionBody(body);
    m_scene.addNode(rectangleNode);

    groundShape.setSize({ 1280.f, 200.f });
    auto groundNode = std::make_unique<Node>("groundNode");
    groundNode->setDrawable(&groundShape);
    groundNode->setPosition(0.f, 760.f);
    auto gb = m_collisionWorld.addBody(CollisionWorld::Body::Type::Solid, { {}, { 1280.f, 200.f } });
    groundNode->setCollisionBody(gb);
    m_scene.addNode(groundNode);
}

bool GameState::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        body->applyForce({ 60.f, 0.f });
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        body->setPosition({ 400.f, 400.f });

    m_collisionWorld.step(dt);

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