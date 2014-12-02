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

#include <Node.hpp>
#include <CollisionWorld.hpp>
#include <Util.hpp>
#include <BodyState.hpp>

#include <cassert>
//#include <iostream>

namespace
{
    const float sensorSize = 10.f;
    const float defaultStrength = 50.f;
}

CollisionWorld::Body::Body(Type type, const sf::Vector2f& size)
    : m_type            (type),
    m_centre            (size / 2.f),
    m_node              (nullptr),
    m_aabb              ({}, size),
    m_footSenseCount    (0u),
    m_gravityAmount     (1.f),
    m_friction          (0.86f),
    m_health            (defaultStrength),
    m_strength          (defaultStrength)
{
    switch (type)
    {
    case Type::Block:
        m_state = std::make_unique<BlockStateAir>(this);
        m_friction = 0.8f;
        break;
    case Type::Npc:
        m_state = std::make_unique<NpcStateAir>(this);
        m_gravityAmount = 0.25f;
        break;
    case Type::Player:
        m_state = std::make_unique<PlayerStateAir>(this);
        break;
    case Type::Solid:
        m_state = std::make_unique<SolidState>(this);
        break;
    default: break;
    }
    assert(m_state);

    //set up perepheral sensor boxes
    m_footSensor.width = size.x;
    m_footSensor.height = sensorSize;
    m_footSensor.top = size.y;
}

CollisionWorld::Body::~Body()
{
    if (m_node)
        m_node->setCollisionBody(nullptr);
}

//public
void CollisionWorld::Body::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_aabb.left = position.x;
    m_aabb.top = position.y;

    m_footSensor.left = position.x;
    m_footSensor.top = position.y + m_aabb.height;
}

void CollisionWorld::Body::applyForce(const sf::Vector2f& force)
{
    m_velocity += m_state->vetForce(force);
}

void CollisionWorld::Body::setGravityAmount(float amount)
{
    m_gravityAmount = amount;
}

void CollisionWorld::Body::setFriction(float friction)
{
    assert(friction >= 0 && friction <= 1);
    m_friction = friction;
}

float CollisionWorld::Body::getFriction() const
{
    return m_friction;
}

void CollisionWorld::Body::setStrength(float strength)
{
    m_strength = strength;
}

CollisionWorld::Body::Type CollisionWorld::Body::getType() const
{
    return m_type;
}

sf::Vector2f CollisionWorld::Body::getCentre() const
{
    return m_position + m_centre;
}

bool CollisionWorld::Body::contains(const sf::Vector2f& point) const
{
    return m_aabb.contains(point);
}

//private
void CollisionWorld::Body::step(float dt)
{
    //state controls the actual force amount
    m_state->update(dt);

    //then we apply whatever force there is
    //-------------------------
    auto stepSpeed = Util::Vector::length(m_velocity) * dt;
    auto stepVelocity = Util::Vector::normalise(m_velocity) * stepSpeed;

    move(stepVelocity);

    if (m_node)
        m_node->setWorldPosition(m_position);
    //--------------------------

    //check to see if a collision has resulted in a new state
    if (m_nextState)
    {
        m_nextState.swap(m_state);
        m_nextState.reset();
    }

    //update strength value or kill if no health
    if (m_health < 0)
    {
        destroy();
    }
    else if (m_health < m_strength)
    {
        m_health += m_strength * dt;
        //std::cout << m_health << std::endl;
    }

}

void CollisionWorld::Body::move(const sf::Vector2f& amount)
{
    m_position += amount;
    m_aabb.left = m_position.x;
    m_aabb.top = m_position.y;

    m_footSensor.left = m_position.x;
    m_footSensor.top = m_position.y + m_aabb.height;
}

void CollisionWorld::Body::applyGravity(const sf::Vector2f& gravity)
{
    m_velocity += gravity * m_gravityAmount;
}

void CollisionWorld::Body::destroy()
{
    game::Event evt;
    evt.type = game::Event::Despawn;
    evt.despawn.type = Category::None;
    notify(*this, evt);
}