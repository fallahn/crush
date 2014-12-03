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

//state class definitions for block objects

#include <BodyState.hpp>
#include <Util.hpp>

#include <iostream>

//------------------------------------------
void BlockStateAir::update(float dt)
{
    //simply negate sideways movement and allow gravity to do its thing
    auto vel = getVelocity();
    vel.x = 0.f;
    setVelocity(vel);
}

void BlockStateAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        setState<BlockStateGround>();
        setParentCategory(Category::Block); //reset any previous owners
        break;

    default: break;
    }
}
//-------------------------------------------
void BlockStateGround::update(float dt)
{  
    auto vel = getVelocity();
    vel.y = 0.f;

    vel.x *= getFriction(); //TODO equate dt into this
    setVelocity(vel);

    if (getFootSenseCount() == 0u)
    {
        //nothing underneath so should be falling
        setState<BlockStateAir>();

        //TODO should set this to not grabbed, but previously owned
    }
}

void BlockStateGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Block:
    //case CollisionWorld::Body::Type::Player:
        if (Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && manifold.x != 0.f) //prevents shifting vertically
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({});
        }
        break;
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        break;
    case CollisionWorld::Body::Type::Player:
        other->applyForce(getVelocity());

        //fall if play pushed block out from underneath
        if (getFootSenseCount() <= 1u
            && (manifold.y * manifold.z) < 0.f)
        {
            setState<BlockStateAir>();
            setParentCategory(Category::Block);
        }
        break;
    default: break;
    }
}
//-------------------------------------------
void SolidState::update(float dt)
{
    setVelocity({});
}

void SolidState::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{

}
