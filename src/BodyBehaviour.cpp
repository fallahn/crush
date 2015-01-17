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

#include <BodyBehaviour.hpp>
#include <Node.hpp>

#include <cassert>
#include <iostream>

BodyBehaviour::BodyBehaviour(CollisionWorld::Body* b)
    : m_body(b)
{
    assert(b);
}

CollisionWorld::Body* BodyBehaviour::getBody() const
{
    return m_body;
}

const sf::Vector2f& BodyBehaviour::getVelocity() const
{
    return m_body->m_velocity;
}

void BodyBehaviour::setVelocity(const sf::Vector2f& vel)
{
    m_body->m_velocity = vel;
}

void BodyBehaviour::move(const sf::Vector2f& amount)
{
    m_body->move(amount);
    if (m_body->m_parent)
    {
        m_body->m_parent->move(amount);
        m_body->m_parent->m_velocity = {};
    }
}

sf::Uint16 BodyBehaviour::getFootSenseCount() const
{
    return m_body->m_footSenseCount;
}

sf::Uint32 BodyBehaviour::getFootSenseMask() const
{
    return m_body->m_footSenseMask;
}

float BodyBehaviour::getFriction() const
{
    return m_body->m_friction;
}

Category::Type BodyBehaviour::getParentCategory() const
{
    if (m_body->m_node)
        return static_cast<Category::Type>(m_body->m_node->getCategory());
    else return Category::None;
}

void BodyBehaviour::setParentCategory(Category::Type type)
{
    if (m_body->m_node) m_body->m_node->setCategory(type);
}

void BodyBehaviour::kill()
{
    m_body->destroy();
}

void BodyBehaviour::damage(float amount, CollisionWorld::Body* damager)
{
    m_body->m_health -= amount;

    //std::cout << m_body->m_health << std::endl;

    //check if health now zero and raise event declaring who did the killing
    if (m_body->m_health <= 0)
    {
        //TODO this event sometimes gets raised twice because the
        //body is killed by two other bodies intersecting it.
        Event e;
        e.node.action = Event::NodeEvent::KilledNode;
        e.node.type = damager->getParentCategory();
        e.node.target = getParentCategory();

        if (e.node.type & (Category::LastTouchedOne | Category::GrabbedOne))
        {
            e.node.owner = Category::PlayerOne;
            e.node.type = Category::Block; //remove the flags from the event, else it might not get parsed
        }
        else if (e.node.type & (Category::LastTouchedTwo | Category::GrabbedTwo))
        {
            e.node.owner = Category::PlayerTwo;
            e.node.type = Category::Block;
        }
        else
        {
            e.node.owner = e.node.target;//assume player crushed self
        }

        e.type = Event::Node;
        raiseEvent(e, damager);
    }
}

void BodyBehaviour::raiseEvent(const Event& evt, CollisionWorld::Body* target)
{
    if(!target) m_body->notify(*m_body, evt);
    else target->notify(*target, evt);
}

bool BodyBehaviour::deleted() const
{
    return m_body->deleted();
}

bool BodyBehaviour::hasChild(CollisionWorld::Body::Type t)
{
    return m_body->hasChild(t);
}