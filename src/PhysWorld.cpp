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

#include <PhysWorld.hpp>
#include <Node.hpp>
#include <Util.hpp>

PhysWorld::PhysWorld(float gravity)
    :m_gravity  (0.f, gravity)
{

}


//public
PhysWorld::PhysObject* PhysWorld::addObject(sf::FloatRect size, const PhysWorld::PhysData& pd)
{
    auto po = std::make_unique<PhysObject>(size, pd);
    m_objects.push_back(std::move(po));
    //adding new objects would invalidate existing references
    //so we need to return the underlying pointer
    return m_objects.back().get();
}

void PhysWorld::step(float dt)
{
    //check for deleted objects and remove them
    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const PhysObject::Ptr& p)
    {
        return p->deleted();
    }), m_objects.end());
    

    //check for collision pairs and add to list
    //TODO we could narrow this down with space partitioning
    //like a quad tree, but probably not necessary in this game
    m_collisionPairs.clear();
    for (const auto& poA : m_objects)
    {
        for (const auto& poB : m_objects)
        {
            if (poA.get() != poB.get() &&
                collision(poA, poB))
            {
                //minmax assures that as the lowest values is always first in the set
                //that each collision pair only gets inserted once
                m_collisionPairs.insert(std::minmax(poA.get(), poB.get()));
            }
        }
    }

    //resolve collision for each pair
    for (const auto& pair : m_collisionPairs)
        resolveCollision(pair);

    //update any parent node positions
    for (auto& po : m_objects)
    {
        po->addForce(m_gravity);
        po->step(dt);
    }
}

//private
bool PhysWorld::collision(const PhysWorld::PhysObject::Ptr& a, const PhysWorld::PhysObject::Ptr& b)
{
    //separate collision testing function so it will be easier
    //to add circular objects later on if needed
    return a->m_aabb.intersects(b->m_aabb);
}

void PhysWorld::resolveCollision(const CollisionPair& cp)
{
    auto relativeVel = cp.second->m_velocity - cp.first->m_velocity;

    //all our objects are rectangluar so the collision normal
    //is only going to be one of 4 directions (ironically this
    //would be easier to calculate for circles)
    sf::Vector2f collisionNormal = getCollisionNormal(cp);

    float normalVel = Util::Vector::dot(relativeVel, collisionNormal);
    if (normalVel > 0) return; //skip if moving away

    float e = std::min(cp.first->m_physData.restitution, cp.second->m_physData.restitution);

    float impulse = -(1 + e) * normalVel;
    impulse /= 1 / cp.first->m_physData.mass / cp.second->m_physData.mass;

    sf::Vector2f impulseVec = impulse * collisionNormal;
    cp.first->m_velocity -= cp.first->m_physData.inverseMass * impulseVec;
    cp.second->m_velocity += cp.second->m_physData.inverseMass * impulseVec;
}

sf::Vector2f PhysWorld::getCollisionNormal(const CollisionPair& cp)
{
    return sf::Vector2f(0.f, 1.f);
}