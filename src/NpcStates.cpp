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

#include <BodyState.hpp>

//------------------------------------------
void NpcStateAir::update(float dt)
{
    //reduce lateral velocity so sideways movement in minimal
    auto vel = getVelocity();
    vel.x *= 0.6f;
    setVelocity(vel);
}

void NpcStateAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType)
{
    switch (otherType)
    {
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        //jump up from ground, or away from walls
        //TODO when squished under or between blocks
        move(sf::Vector2f( manifold.x, manifold.y ) * manifold.z);
    {
        auto vel = getVelocity();
        if (manifold.y != 0)
            vel.y -= 2200.f;
        if (manifold.x != 0)
            vel.x = -vel.x;
        setVelocity(vel);
    }
        break;
    case CollisionWorld::Body::Type::Player:
    {
        //kill self if player is above
        if (manifold.y * manifold.z > 0) kill();

        //player state kills player if npc is above

        //TODO what about side ways?
    }
    break;
    case CollisionWorld::Body::Type::Npc:
        //steer away from other baddies

        break;

    default: break;
    }
}

//-------------------------------------------