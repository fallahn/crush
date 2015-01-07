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

#include <FreeFormBehaviour.hpp>
#include <Util.hpp>

namespace
{
    const float maxSpeed = 900000.f;
}

void FreeFormBehaviourAir::update(float dt)
{
    //hack to prevent as much tunnelling as possible
    const float limit = maxSpeed / getBody()->getSpeed();
    auto vel = getVelocity() * std::min(1.f, limit);
    setVelocity(vel * getFriction());
}

void FreeFormBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Water:
        setBehaviour<FreeFormBehaviourWater>();

        //raise splash event
        Event evt;
        evt.type = Event::Node;
        evt.node.type = Category::Water;
        evt.node.action = Event::NodeEvent::HitWater;
        evt.node.positionX = getBody()->getCentre().x;
        evt.node.positionY = getBody()->getCentre().y + (getBody()->getSize().y / 2.f);
        evt.node.speed = getVelocity().y;
        raiseEvent(evt, other);

        break;
    case CollisionWorld::Body::Block:
    case CollisionWorld::Body::Solid:
        //if (getFootSenseMask() & (CollisionWorld::Body::Solid | CollisionWorld::Body::Block))
        //    setBehaviour<FreeFormBehaviourGround>();

        //TODO raise event?

    default:
        sf::Vector2f normal(manifold.x, manifold.y);
        move(normal * manifold.z);

        auto vel = getVelocity();
        vel = Util::Vector::reflect(vel, normal) * getFriction();
        vel += other->getVelocity();
        setVelocity(vel);

        break;
    }
}

//--------------------------------------

void FreeFormBehaviourGround::update(float dt)
{
    const float limit = maxSpeed / getBody()->getSpeed();
    auto vel = getVelocity() * std::min(1.f, limit);
    setVelocity({vel.x * getFriction(), 0.f});

    if ((getFootSenseMask() & (CollisionWorld::Body::Solid | CollisionWorld::Body::Block)) == 0)
        setBehaviour<FreeFormBehaviourAir>();
}

void FreeFormBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Water:
        break;
    default:
        sf::Vector2f normal(manifold.x, manifold.y);
        move(normal * manifold.z);

        auto vel = getVelocity();
        vel = Util::Vector::reflect(vel, normal) * getFriction();
        vel += other->getVelocity();
        setVelocity(vel);
        
        break;
    }
}

//--------------------------------------

void FreeFormBehaviourWater::update(float dt)
{
    setVelocity(getVelocity() * 0.4f);

    if (getFootSenseMask() & (CollisionWorld::Body::Type::Solid | CollisionWorld::Body::Block))
        setBehaviour<FreeFormBehaviourGround>();
}

void FreeFormBehaviourWater::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    if (other->getType() != CollisionWorld::Body::Water)
    {
        sf::Vector2f normal(manifold.x, manifold.y);
        move(normal * manifold.z);

        auto vel = getVelocity();
        vel = Util::Vector::reflect(vel, normal);
        vel += other->getVelocity();
        setVelocity(vel);
    }
}
