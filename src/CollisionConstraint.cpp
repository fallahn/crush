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

#include <CollisionWorld.hpp>
#include <Util.hpp>

#include <cassert>
#include <iostream>

CollisionWorld::Constraint::Constraint(CollisionWorld::Body* a, CollisionWorld::Body* b, float length)
    : m_bodyA   (a),
    m_bodyB     (b),
    m_length    (length)
{
    assert(a);
    assert(b);
}

//public
void CollisionWorld::Constraint::update(float dt)
{
    //TODO replace with observer?
    /*if (m_bodyA->deleted() || m_bodyB->deleted())
    {
        deleteObject();
    }*/

    //TODO square root optimise?
    auto constraintVec = m_bodyB->getCentre() - m_bodyA->getCentre();
    float constraintLength = Util::Vector::length(constraintVec);
    auto constraintUnit = constraintVec / constraintLength;

    float relativeVelocity = Util::Vector::dot((m_bodyB->m_velocity - m_bodyA->m_velocity), constraintUnit);
    float relativeDistance = constraintLength - m_length;

    sf::Vector2f force(constraintUnit * (relativeVelocity + relativeDistance));

    m_bodyA->applyForce(force);
    m_bodyB->applyForce(-force);

}