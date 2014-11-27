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
#include <DebugShape.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace
{
    Camera sceneCam;
    sf::RectangleShape rectangleShape;// ({ 200.f, 120.f });
    sf::RectangleShape groundShape;
    sf::RectangleShape wallShape;

    CollisionWorld::Body* body = nullptr;

    DebugShape debugShape;
}

GameState::GameState(StateStack& stack, Context context)
    : State             (stack, context),
    m_collisionWorld    (50.f)
{
    getContext().renderWindow->setTitle("Game Screen");
    
    auto camNode = std::make_unique<Node>("camNode");
    sceneCam.setView({ {}, { 1920.f, 1080.f } });
    camNode->setCamera(&sceneCam);
    camNode->setPosition(sceneCam.getView().getSize() / 2.f);
    m_scene.addNode(camNode);

    rectangleShape.setFillColor(sf::Color::Transparent);
    rectangleShape.setOutlineColor(sf::Color::Red);
    rectangleShape.setOutlineThickness(-3.f);
    groundShape = rectangleShape;
    wallShape = rectangleShape;

    debugShape.setSize({ 200.f, 120.f });
    debugShape.setColour(sf::Color::Red);

    groundShape.setSize({ 1920.f, 50.f });
    auto groundNode = std::make_unique<Node>("groundNode");
    groundNode->setDrawable(&groundShape);
    groundNode->setPosition(0.f, 1030.f);
    auto gb = m_collisionWorld.addBody(CollisionWorld::Body::Type::Solid, groundShape.getSize());
    groundNode->setCollisionBody(gb);
    m_scene.addNode(groundNode);

    wallShape.setSize({ 50.f, 1030.f });
    auto leftWallNode = std::make_unique<Node>("leftWall");
    leftWallNode->setDrawable(&wallShape);
    leftWallNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Solid, wallShape.getSize()));
    m_scene.addNode(leftWallNode);

    auto rightWallNode = std::make_unique<Node>("rightWall");
    rightWallNode->setDrawable(&wallShape);
    rightWallNode->setPosition(1880.f, 0.f);
    rightWallNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Solid, wallShape.getSize()));
    m_scene.addNode(rightWallNode);
}

bool GameState::update(float dt)
{
    if (body)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            body->applyForce({ -60.f, 0.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            body->applyForce({ 60.f, 0.f });
    }
    m_collisionWorld.step(dt);

    return true;
}

void GameState::draw()
{
    getContext().renderWindow->draw(m_scene);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    switch (evt.type)
    {
    case sf::Event::MouseButtonReleased:
        switch(evt.mouseButton.button)
        {
        case sf::Mouse::Left:
            auto position = getContext().renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*getContext().renderWindow));
            addBlock(position);
            break;
        }
        break;
    }
    
    return true;
}



//private
void GameState::addBlock(const sf::Vector2f& position)
{
    auto rectangleNode = std::make_unique<Node>("rectangleNode");
    rectangleNode->setPosition(position);
    rectangleNode->setDrawable(&debugShape);
    
    body = m_collisionWorld.addBody(CollisionWorld::Body::Type::Block, debugShape.getSize());
    rectangleNode->setCollisionBody(body);
    m_scene.addNode(rectangleNode);
}