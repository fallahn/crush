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

#include <ItemBehaviour.hpp>

void ItemBehaviourAir::update(float dt)
{
    auto vel = getVelocity();
    vel.x *= 0.9f;
    setVelocity(vel);
}

void ItemBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});

        if (getFootSenseMask() & (CollisionWorld::Body::Block | CollisionWorld::Body::Solid))
        {
            setState<ItemBehaviourGround>();
        }
        break;
    case CollisionWorld::Body::Player:
        //TODO let player collect
        break;
    default: break; //NPCs don't do anything
    }
}

//--------------------------------------

void ItemBehaviourGround::update(float dt)
{
    auto vel = getVelocity();
    vel.x *= getFriction();
    vel.y = 0.f;
    setVelocity(vel);

    if (getFootSenseCount() == 0)
        setState<ItemBehaviourAir>();
}

void ItemBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Block:
        //might be fun to accidentally knock items with a block or something
        setVelocity({ manifold.x * manifold.z, 0.f });
    case CollisionWorld::Body::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        break;

    case CollisionWorld::Body::Player:
        //TODO let player collect
        break;
    default: break; //NPCs don't do anything
    }
}