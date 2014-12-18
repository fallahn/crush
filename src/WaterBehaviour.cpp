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

#include <WaterBehaviour.hpp>

void WaterBehaviourAir::update(float dt)
{
    //water shouldn't really be in this state long, it's just
    //to make sure it 'settles' properly when the map is created

    auto vel = getVelocity();
    vel.x = 0.f;
    setVelocity(vel);
}

void WaterBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    //switch to idle state when meeting a solid object
    if (other->getType() == CollisionWorld::Body::Type::Solid)
    {
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        setBehaviour<WaterBehaviourGround>();
    }
}

//-------------------

void WaterBehaviourGround::update(float dt)
{
    setVelocity({});
    //just kill gravity effects
}

void WaterBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    //nothing to do for now
    //maybe later go splish.
}