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

#include <BlockBehaviour.hpp>
#include <Util.hpp>

#include <iostream>

//-------------------------------------------
void BlockBehaviourAir::update(float dt)
{
    //simply negate sideways movement and allow gravity to do its thing
    auto vel = getVelocity();
    vel.x = 0.f;
    setVelocity(vel);

    sf::Int32 cat = getParentCategory();
    if (cat & (Category::CarriedOne | Category::CarriedTwo))
    {
        //can't carry blocks which are in the air 
        Event e;
        e.type = Event::Player;
        e.player.action = Event::PlayerEvent::Dropped;
        e.player.playerId = (cat & (Category::CarriedOne)) ? Category::PlayerOne : Category::PlayerTwo;
        e.player.positionX = 0.f;
        e.player.positionY = 0.f;
        raiseEvent(e);
    }

    if (getFootSenseMask() == CollisionWorld::Body::Type::Water) //touches water only
    {
        setBehaviour<BlockBehaviourWater>();
    }
}

void BlockBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Block:
        //setParentCategory(Category::Block); //reset any previous owners
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        setBehaviour<BlockBehaviourGround>();
        break;
    default: break;
    }
}
//-------------------------------------------

void BlockBehaviourGround::update(float dt)
{  
    auto vel = getVelocity();
    vel.y = 0.f;

    vel.x *= getFriction(); //TODO equate dt into this
    setVelocity(vel);

    if ((getFootSenseMask() & (CollisionWorld::Body::Type::Block | CollisionWorld::Body::Type::Solid)) == 0)
    {
        //nothing underneath so should be falling
        setBehaviour<BlockBehaviourAir>();

        //TODO should set this to not grabbed, but previously owned
    }

    sf::Int32 cat = getParentCategory();
    if (cat & (Category::CarriedOne | Category::CarriedTwo))
    {
        setBehaviour<BlockBehaviourCarry>();
    }
}

void BlockBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Block:
        if ((/*Util::Vector::lengthSquared(getVelocity()) > 0.2f
            &&*/ manifold.x != 0.f)) //prevents shifting vertically
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

        //fall if player pushed block out from underneath
        if (getFootSenseCount() <= 1u
            && (manifold.y * manifold.z) < 0.f)
        {
            setBehaviour<BlockBehaviourAir>();
            setParentCategory(Category::Block);
        }
        break;
    default: break;
    }
}
//-------------------------------------------
void BlockBehaviourCarry::update(float dt)
{
    //auto vel = getVelocity();
    //vel.y = 0.f; //cancel gravity
    //setVelocity(vel);
    setVelocity({});

    if ((getParentCategory() & (Category::CarriedOne | Category::CarriedTwo)) == 0)
    {
        //no longer being carried
        setBehaviour<BlockBehaviourAir>();
    }
}

void BlockBehaviourCarry::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Player:
    case CollisionWorld::Body::Type::Npc:

    case CollisionWorld::Body::Type::Block:
    case CollisionWorld::Body::Type::Solid:
        //if block above then drop block by raising player drop event
        if (manifold.y != 0)//(manifold.y * manifold.z) > 0.f)
        {
            Event e;
            e.type = Event::Player;
            e.player.action = Event::PlayerEvent::Dropped;
            e.player.playerId = (getParentCategory() & (Category::CarriedOne)) ? Category::PlayerOne : Category::PlayerTwo;
            e.player.positionX = other->getCentre().x;
            e.player.positionY = other->getCentre().y;
            raiseEvent(e);
        }
    //case CollisionWorld::Body::Type::Player:
    //case CollisionWorld::Body::Type::Npc:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});

        break;
    default: break;
    }
}
//-------------------------------------------
void BlockBehaviourWater::update(float dt)
{
    auto vel = getVelocity();
    vel *= 0.45f; //sink slowly
    setVelocity(vel);
}

void BlockBehaviourWater::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Block:
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        setBehaviour<BlockBehaviourGround>();
        break;
    case CollisionWorld::Body::Type::Water:
        if(!m_splashed)
        {
            //raise splash event
            Event evt;
            evt.type = Event::Node;
            evt.node.type = Category::Water;
            evt.node.action = Event::NodeEvent::HitWater;
            evt.node.positionX = getBody()->getCentre().x;
            evt.node.positionY = getBody()->getCentre().y + (getBody()->getSize().y / 2.f);
            evt.node.speed = getVelocity().y;
            raiseEvent(evt, other);

            m_splashed = true;
        }
        break;
    default: break;
    }
}


//-------------------------------------------
void SolidBehaviour::update(float dt)
{
    setVelocity({});
}

void SolidBehaviour::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{

}
