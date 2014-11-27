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
PhysWorld::Body* PhysWorld::addObject(sf::FloatRect size, const PhysWorld::BodyData& pd)
{
    auto po = std::make_unique<Body>(size, pd);
    m_bodies.push_back(std::move(po));
    //adding new objects would invalidate existing references
    //so we need to return the underlying pointer
    return m_bodies.back().get();
}

void PhysWorld::step(float dt)
{
    //check for deleted objects and remove them
    m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [](const Body::Ptr& p)
    {
        return p->deleted();
    }), m_bodies.end());
    

    //check for collision pairs and add to list
    //TODO we could narrow this down with space partitioning
    //like a quad tree, but probably not necessary in this game
    m_collisionPairs.clear();
    for (const auto& poA : m_bodies)
    {
        for (const auto& poB : m_bodies)
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
    for (auto& po : m_bodies)
    {
        if(po->m_bodyData.m_type == BodyType::Dynamic)
            //&& Util::Vector::lengthSquared(po->m_velocity) > 0.1f)
            po->m_velocity += m_gravity;
        po->step(dt);
    }
}

//private
bool PhysWorld::collision(const PhysWorld::Body::Ptr& a, const PhysWorld::Body::Ptr& b)
{
    //separate collision testing function so it will be easier
    //to add circular objects later on if needed
    return a->m_aabb.intersects(b->m_aabb);
}

void PhysWorld::resolveCollision(const CollisionPair& cp)
{
    auto relativeVelocity = cp.second->m_velocity - cp.first->m_velocity;
    auto manifold = getManifold(cp);

    float normalVel = Util::Vector::dot(relativeVelocity, manifold.normal);
    if (normalVel >= 0) return; //skip if moving away

    float e = std::min(cp.first->m_bodyData.m_restitution, cp.second->m_bodyData.m_restitution);

    float impulse = -(1.f + e) * normalVel;
    impulse /= (cp.first->m_bodyData.m_inverseMass + cp.second->m_bodyData.m_inverseMass);

    sf::Vector2f impulseVec = impulse * manifold.normal;
    float totalMass = cp.first->m_bodyData.m_mass + cp.second->m_bodyData.m_mass;
    cp.first->m_velocity -= (cp.first->m_bodyData.m_inverseMass / totalMass) * impulseVec;
    cp.second->m_velocity += (cp.second->m_bodyData.m_inverseMass / totalMass) * impulseVec;

    //stop sinking - TODO send to sleep?
    /*sf::Vector2f correction = std::max(0.f, manifold.penetration - 0.01f) / (cp.first->m_bodyData.m_inverseMass + cp.second->m_bodyData.m_inverseMass) * manifold.normal * 0.3f;
    cp.first->move(-correction * cp.first->m_bodyData.m_inverseMass);
    cp.second->move(correction * cp.second->m_bodyData.m_inverseMass);*/
}

PhysWorld::CollisionManifold PhysWorld::getManifold(const CollisionPair& cp)
{
    sf::Vector2f collisionNormal = cp.second->m_position - cp.first->m_position;
    
    //TODO this is just plain WRONG. use the AABB intersection function to return
    //the correct overlap (thankyou laurent)

    float aExtent = cp.first->m_aabb.width / 2.f;
    float bExtent = cp.second->m_aabb.width / 2.f;
    float xOverlap = aExtent + bExtent - std::abs(collisionNormal.x);

    aExtent = cp.first->m_aabb.height / 2.f;
    bExtent = cp.second->m_aabb.height / 2.f;
    float yOverlap = aExtent + bExtent - std::abs(collisionNormal.y);

    CollisionManifold manifold;
    if (xOverlap < yOverlap)
    {
        manifold.normal.x = (collisionNormal.x < 0) ? -1.f : 1.f;
        manifold.penetration = xOverlap;
    }
    else
    {
        manifold.normal.y = (collisionNormal.y < 0) ? -1.f : 1.f;
        manifold.penetration = yOverlap;
    }
 
    return manifold;
}