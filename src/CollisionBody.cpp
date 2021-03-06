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

#include <Node.hpp>
#include <CollisionWorld.hpp>
#include <Util.hpp>
#include <BlockBehaviour.hpp>
#include <NpcBehaviour.hpp>
#include <PlayerBehaviour.hpp>
#include <WaterBehaviour.hpp>
#include <ItemBehaviour.hpp>
#include <FreeFormBehaviour.hpp>
#include <AnchorBehaviour.hpp>

#include <cassert>
#include <iostream>

namespace
{
    const float sensorSize = 3.f;
    const float defaultStrength = 35.f;
    //TODO fix magic numbers - basically default view size with 100 unit padding
    const sf::FloatRect worldSize = { { -100.f, -100.f }, { 2020.f, 1280.f } };
    const float invincibilityTime = 4.f;
    const float turboSpeed = 1700000.f; //bodies moving faster than this smoke :)
}

CollisionWorld::Body::Body(Type type, const sf::Vector2f& size)
    : m_type            (type),
    m_centre            (size / 2.f),
    m_node              (nullptr),
    m_aabb              ({}, size),
    m_footSenseCount    (0u),
    m_footSenseMask     (0u),
    m_gravityAmount     (1.f),
    m_friction          (0.86f),
    m_health            (defaultStrength),
    m_strength          (defaultStrength),
    m_parent            (nullptr),
    m_dead              (false),
    m_invincible        (false),
    m_invincibilityCount(0.f),
    m_lastSpeed         (0.f)
{
    switch (type)
    {
    case Type::Block:
        m_behaviour = std::make_unique<BlockBehaviourAir>(this);
        m_friction = 0.84f;
        break;
    case Type::Npc:
        m_behaviour = std::make_unique<NpcBehaviourAir>(this);
        m_gravityAmount = 0.15f;
        m_strength = 40.f;
        break;
    case Type::Player:
        m_behaviour = std::make_unique<PlayerBehaviourAir>(this);
        m_invincible = true;
        break;
    case Type::Solid:
        m_behaviour = std::make_unique<SolidBehaviour>(this);
        break;
    case Type::Water:
        m_behaviour = std::make_unique<WaterBehaviourAir>(this);
        break;
    case Type::Item:
        m_behaviour = std::make_unique<ItemBehaviourAir>(this);
        break;
    case Type::FreeForm:
        m_behaviour = std::make_unique<FreeFormBehaviourAir>(this);
        m_friction = 0.97f;
        m_strength = 10.f;
        break;
    case Type::Anchor:
        m_behaviour = std::make_unique<AnchorBehaviour>(this);
        break;
    default: break;
    }
    assert(m_behaviour);

    //set up perepheral sensor boxes
    m_footSensor.width = size.x;
    m_footSensor.height = sensorSize;
    m_footSensor.top = size.y;
}

CollisionWorld::Body::~Body()
{
    for (auto& c : m_children)
        c.first->m_parent = nullptr; 

    if (m_parent)
    {
        m_parent->removeChild(this);
        m_parent = nullptr;
    }
    
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

    if (m_node) m_node->setWorldPosition(m_position);
}

void CollisionWorld::Body::applyForce(const sf::Vector2f& force)
{
    m_velocity += m_behaviour->vetForce(force);
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

Category::Type CollisionWorld::Body::getParentCategory() const
{
    if (m_node)
        return static_cast<Category::Type>(m_node->getCategory());
    else
        return Category::None;
}

sf::Vector2f CollisionWorld::Body::getSize() const
{
    return { m_aabb.width, m_aabb.height };
}

sf::Vector2f CollisionWorld::Body::getCentre() const
{
    return m_position + m_centre;
}

bool CollisionWorld::Body::contains(const sf::Vector2f& point) const
{
    return m_aabb.contains(point);
}

void CollisionWorld::Body::addChild(CollisionWorld::Body* b, const sf::Vector2f& relPosition)
{
    if (b->m_parent != this)
    {
        m_children[b] = relPosition;// -b->m_centre;
        if (b->m_parent)
        {
            b->m_parent->removeChild(b);
        }
        b->m_parent = this;
    }

    //std::cerr << relPosition.x << ", " << relPosition.y << std::endl;
}

void CollisionWorld::Body::removeChild(CollisionWorld::Body* b)
{
    auto result = m_children.find(b);
    if (result != m_children.end())
    {
        b->m_parent = nullptr;
        b->m_velocity = m_velocity;
        if (b->m_node) b->setPosition(b->m_node->getWorldPosition());
        m_children.erase(result);
    }
}

void CollisionWorld::Body::detatch()
{
    if (m_parent)
    {
        m_parent->removeChild(this);
    }
}

void CollisionWorld::Body::flipChildren()
{
    for (auto& c : m_children)
        c.second.x = -c.second.x;
}

float CollisionWorld::Body::getSpeed() const
{
    return Util::Vector::lengthSquared(getVelocity());
}

const sf::Vector2f& CollisionWorld::Body::getVelocity() const
{
    if (m_parent) return m_parent->getVelocity();
    return m_velocity;
}

//private
void CollisionWorld::Body::step(float dt)
{
    //state controls the actual force amount
    m_behaviour->update(dt);

    //then we apply whatever force there is
    //-------------------------
    auto stepSpeed = Util::Vector::length(m_velocity) * dt;
    auto stepVelocity = Util::Vector::normalise(m_velocity) * stepSpeed;

    move(stepVelocity);

    //check to see if still in world bounds
    if (!worldSize.contains(getCentre()))
    {
        setPosition({ worldSize.width / 2.f, worldSize.top + 20.f });
        m_velocity.y = 0.f;
    }

    //make sure bodies come to complete halt
    float ls = Util::Vector::lengthSquared(m_velocity);
    if (ls != 0 && ls < 1.5f)
    {
        m_velocity = {};
        //std::cout << "stopped body" << std::endl;
    }

    //update all the child bodies
    for (auto& c : m_children)
    {
        c.first->setPosition(getCentre() + (c.second - c.first->m_centre));
    }

    if (m_node)
        m_node->setWorldPosition(m_position);
    //--------------------------

    //check to see if a collision has resulted in a new state
    if (m_nextBehaviour)
    {
        m_nextBehaviour.swap(m_behaviour);
        m_nextBehaviour.reset();
    }

    //update strength value or kill if no health
    if (m_health < 0)
    {
        destroy();
    }
    else if (m_health < m_strength)
    {
        m_health += m_strength * dt;
    }


    //check for invincibility time out
    if (m_invincible)
    {
        m_invincibilityCount += dt;
        if (m_invincibilityCount > invincibilityTime)
        {
            m_invincible = false;

            //send event
            Event e;
            e.type = Event::Node;
            e.node.action = Event::NodeEvent::InvincibilityExpired;
            e.node.positionX = m_position.x;
            e.node.positionY = m_position.y;
            notify(*this, e);
        }
    }

    //check speed for any effects it may have - TODO this doesn't work amazingly, may revisit some time
    /*const float currentSpeed = getSpeed();
    if (currentSpeed > turboSpeed
        && m_lastSpeed < turboSpeed)
    {
        Event e;
        e.type = Event::Node;
        e.node.action = Event::NodeEvent::WentTurbo;
        e.node.positionX = m_position.x;
        e.node.positionY = m_position.y;
        notify(*this, e);
    }
    else if (currentSpeed < turboSpeed
        && m_lastSpeed > turboSpeed)
    {
        Event e;
        e.type = Event::Node;
        e.node.action = Event::NodeEvent::LeftTurbo;
        e.node.positionX = m_position.x;
        e.node.positionY = m_position.y;
        notify(*this, e);
    }
    m_lastSpeed = currentSpeed;*/
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
    //this could possible get called twice on the same body
    //from both being crushed, and losing all health
    if (!m_dead && !m_invincible)
    {
        Event evt;
        evt.type = Event::Node;
        evt.node.type = Category::None;
        evt.node.action = Event::NodeEvent::Despawn;
        notify(*this, evt);
        
        m_dead = true;
    }
}

bool CollisionWorld::Body::hasChild(Type type)
{
    for (const auto& p : m_children)
    {
        if (p.first->getType() == type) return true;
    }
    return false;
}