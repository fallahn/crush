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

#include <CollisionWorld.hpp>
#include <Node.hpp>
#include <Util.hpp>
#include <BodyState.hpp>

CollisionWorld::CollisionWorld(float gravity)
    : m_gravity(0.f, gravity)
{

}

CollisionWorld::Body* CollisionWorld::addBody(CollisionWorld::Body::Type type, const sf::Vector2f& size)
{
    auto b = std::make_unique<Body>(type, size);
    m_bodies.push_back(std::move(b));
    return m_bodies.back().get();
}

void CollisionWorld::step(float dt)
{

    //check for deleted objects and remove them
    m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [](const Body::Ptr& p)
    {
        return p->deleted();
    }), m_bodies.end());

    //check for collision pairs and add to list
    //TODO we could narrow this down with space partitioning
    //like a quad tree, but probably not necessary in this game
    m_collisions.clear();
    for (const auto& poA : m_bodies)
    {
        poA->m_footSenseCount = 0u;
        poA->m_footSenseMask = 0u;
        for (const auto& poB : m_bodies)
        {           
            if (poA.get() != poB.get())
            {
                //primary collision between bounding boxes
                if (poA->m_aabb.intersects(poB->m_aabb))
                {
                    //minmax assures that as the lowest values is always first in the set
                    //that each collision pair only gets inserted once
                    m_collisions.insert(std::minmax(poA.get(), poB.get()));
                }

                //secondary collisions with sensor boxes
                if (poA->m_footSensor.intersects(poB->m_aabb))
                {
                    poA->m_footSenseCount++;
                    poA->m_footSenseMask |= poB->m_type;
                }
            }
        }
    }

    //resolve collision for each pair
    for (const auto& pair : m_collisions)
    {
        //call state resolve
        auto man = getManifold(pair);
        pair.second->m_state->resolve(man, pair.first);
        man.z = -man.z;
        pair.first->m_state->resolve(man, pair.second);
    }

    //update any parent node positions
    for (auto& po : m_bodies)
    {
        po->applyGravity(m_gravity);
        po->step(dt);
    }
}

//private
sf::Vector3f  CollisionWorld::getManifold(const CollisionPair& cp)
{
    sf::Vector2f collisionNormal = cp.second->m_position - cp.first->m_position;
    sf::FloatRect overlap;
    //might seem less eficient than caching the first intersection test
    //but appears to work more accurately
    cp.first->m_aabb.intersects(cp.second->m_aabb, overlap);

    sf::Vector3f manifold;
    if (overlap.width < overlap.height)
    {
        manifold.x = (collisionNormal.x < 0) ? -1.f : 1.f;
        manifold.z = overlap.width;
    }
    else
    {
        manifold.y = (collisionNormal.y < 0) ? -1.f : 1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}