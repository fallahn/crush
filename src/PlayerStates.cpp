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
#include <Util.hpp>

//------------------------------------------
void PlayerStateAir::update(float dt)
{
    auto vel = getVelocity();
    vel.x *= 0.8f; //this should always be <= ground friction?
    setVelocity(vel);
}

void PlayerStateAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType)
{
    switch (otherType)
    {
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({ getVelocity().x, 0.f });
        setState<PlayerStateGround>();
        break;
    case CollisionWorld::Body::Type::Player:
    {
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        auto vel = getVelocity();
        if (manifold.x != 0) //players colliding sideways in air
        {
            setVelocity({ -vel.x, vel.y });
        }
        else
        {
            setVelocity({ vel.x, -vel.y });
        }
    }
        break;
    default: break;
    }
}

sf::Vector2f PlayerStateAir::vetForce(const sf::Vector2f& force)
{
    //basically prevent jumping infinitely, while retaining air control
    return (getFootSenseCount() == 0u) ? sf::Vector2f(force.x, 0.f) : force;
}

//-------------------------------------------
void PlayerStateGround::update(float dt)
{
    auto vel = getVelocity();
    if(vel.y > 0.f) vel.y = 0.f;

    const float friction = 0.86f; //TODO make this variable in state class so it can be changed via gameplay
    vel.x *= friction; //TODO equate dt into this
    setVelocity(vel);

    if (getFootSenseCount() == 0u)
    {
        //nothing underneath so should be falling / jumping
        setState<PlayerStateAir>();
    }
}

void PlayerStateGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType)
{
    switch (otherType)
    {
    case CollisionWorld::Body::Type::Block:
        if (Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && manifold.x != 0.f) //prevents shifting vertically
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({ 0.f, 0.f });
        }
        break;
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        if (manifold.x == 0)
            setVelocity({ getVelocity().x, 0.f }); //carry on moving if we hit ground
        else
            setVelocity({ }); //we hit a wall so stop
        //TODO rather than try resolve wall jumping use side sensors to prevent
        //adding vertical velocity, and velocity in direction of collision
        break;
    case CollisionWorld::Body::Type::Player:
        //move away if side collision 
        if (manifold.x != 0)
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({});
        }
        else if (manifold.y * manifold.z > 0.f)
        {
            //else squish when from above
            kill();
        }
        break;
    default: break;
    }
}