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
#include <PhysWorld.hpp>
#include <Util.hpp>

PhysWorld::PhysObject::PhysObject(sf::FloatRect size, const PhysWorld::PhysData& pd)
    : m_sleeping    (false),
    m_physData      (pd),
    m_node          (nullptr),
    m_aabb          (size)
{

}

PhysWorld::PhysObject::~PhysObject()
{
    if (m_node)
        m_node->setPhysObject(nullptr);
}

const sf::Vector2f& PhysWorld::PhysObject::getPosition() const
{
    return m_position;
}

void PhysWorld::PhysObject::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_aabb.left = position.x;
    m_aabb.top = position.y;
}

void PhysWorld::PhysObject::addForce(const sf::Vector2f& force)
{
    m_velocity += force;
}

void PhysWorld::PhysObject::step(float dt)
{
    if (m_physData.mass == 0) return;
    
    auto stepSpeed = Util::Vector::length(m_velocity) * dt;
    auto stepVelocity = Util::Vector::normalize(m_velocity) * stepSpeed;

    m_position += stepVelocity;
    m_aabb.left = m_position.x;
    m_aabb.top = m_position.y;

    if (m_node)
        m_node->setWorldPosition(m_position);
}