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

#include <PlayerBehaviour.hpp>
#include <Util.hpp>

#include <iostream>

namespace
{
    const float damageMultiplier = 0.47f;
}

//------------------------------------------
void PlayerBehaviourAir::update(float dt)
{
    auto vel = getVelocity();
    vel.x *= 0.8f; //this should always be <= ground friction?
    setVelocity(vel);

    if (vel.y > 0 && m_lastVelocity.y < 0)
    {
        //started falling
        Event e;
        e.type = Event::Type::Player;
        e.player.action = Event::PlayerEvent::Action::StartedFalling;
        raiseEvent(e);
    }
    m_lastVelocity = vel;
}

void PlayerBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Water:
        setBehaviour<PlayerBehaviourWater>();
        break;
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        if (manifold.y * manifold.z < 0) //contact is below so must be standing on something
        {
            setVelocity({ getVelocity().x, 0.f });
            setBehaviour<PlayerBehaviourGround>();

            Event playerEvent;
            playerEvent.type = Event::Player;
            playerEvent.player.playerId = Category::None;
            playerEvent.player.action = Event::PlayerEvent::Landed;
            raiseEvent(playerEvent);
        }
        break;
    case CollisionWorld::Body::Type::Player:
    {
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        auto vel = getVelocity();
        if (manifold.x != 0) //players colliding sideways in air
        {
            setVelocity({ -vel.x, vel.y });
        }
        else if (manifold.y * manifold.z < 0)
        {
            //bounce off players below
            setVelocity({ vel.x, -vel.y * getFriction() });
        }
    }
    break;
    case CollisionWorld::Body::Type::Npc:
    {
        //move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);

        ////bounce off NPCs
        //auto vel = getVelocity();
        //if (manifold.x != 0)
        //    vel.x = -vel.x;
        //if (manifold.y != 0)
        //    vel.y = -vel.y;

        //vel *= getFriction();
        //setVelocity(vel);
        kill();
    }
    break;
    case CollisionWorld::Body::Item:
    {
        Event e;
        e.node.action = Event::NodeEvent::KilledNode;
        e.node.type = getParentCategory();
        e.node.target = Category::Item;
        e.type = Event::Node;
        raiseEvent(e);
    }
        break;
    case CollisionWorld::Body::FreeForm:
    {
        sf::Vector2f normal(manifold.x, manifold.y);
        move(normal * manifold.z);
        setVelocity(Util::Vector::reflect(getVelocity() * getFriction(), normal));
    }
        break;
    default: break;
    }
}

sf::Vector2f PlayerBehaviourAir::vetForce(const sf::Vector2f& force)
{
    //basically prevent jumping infinitely, while retaining air control
    return (getFootSenseCount() == 0u) ? sf::Vector2f(force.x, 0.f) : force;
}

//-------------------------------------------
void PlayerBehaviourGround::update(float dt)
{
    auto vel = getVelocity();
    if(vel.y > 0.f) vel.y = 0.f;

    vel.x *= getFriction(); //TODO equate dt into this
    setVelocity(vel);

    //raise start / stop events
    if (vel.x != 0 && m_lastVelocity.x == 0)
    {
        Event e;
        e.type = Event::Type::Player;
        e.player.action = Event::PlayerEvent::Moved;
        raiseEvent(e);
    }
    else if (vel.x == 0 && m_lastVelocity.x != 0)
    {
        Event e;
        e.type = Event::Type::Player;
        e.player.action = Event::PlayerEvent::Stopped;
        raiseEvent(e);
    }
    m_lastVelocity = vel;

    if ((getFootSenseMask() & (CollisionWorld::Body::Type::Block | CollisionWorld::Body::Type::Solid)) == 0)
    {
        setBehaviour<PlayerBehaviourAir>();

        //let go of whatever we're holding
        Event e;
        e.type = Event::Player;
        e.player.action = Event::PlayerEvent::Released;
        e.player.playerId = getParentCategory();// (cat & Category::GrabbedOne) ? Category::PlayerOne : Category::PlayerTwo;
        auto pos = getBody()->getCentre();
        e.player.positionX = pos.x;
        e.player.positionY = pos.y;
        raiseEvent(e);
    }
}

void PlayerBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Water:
        setBehaviour<PlayerBehaviourWater>();
        break;
    case CollisionWorld::Body::Type::Block:
        if (/*Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && */manifold.x != 0.f) //prevents shifting vertically
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            float yVel = std::min(0.f, getVelocity().y);
            setVelocity({0.f, yVel});    
        
            int cat = other->getParentCategory();
                       
            if ((cat & (Category::CarriedOne | Category::CarriedTwo)) == 0 //don't take damage from blocks being carried,
                && other->getSpeed() > 10.f) //or if they aren't moving
            {
                damage(std::fabs(manifold.z * (damageMultiplier / getFootSenseCount())), other); //always take same damage regardless of blocks touching
            }
            else
            {
                //drop block?
            }
                //std::cerr << manifold.z << std::endl;
        }
        break;
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        if (manifold.x == 0)
            setVelocity({ getVelocity().x, 0.f }); //carry on moving if we hit ground
        else
            setVelocity({}); 
        break;
    case CollisionWorld::Body::Type::Player:
        //move away if side collision 
        if (manifold.x != 0)
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({});
        }

        break;
    case CollisionWorld::Body::Type::Npc:
        //always die
        kill();
        break;
    case CollisionWorld::Body::Item:
    {
        Event e;
        e.node.action = Event::NodeEvent::KilledNode;
        e.node.type = getParentCategory();
        e.node.target = Category::Item;
        e.type = Event::Node;
        raiseEvent(e);
    }
        break;
    case CollisionWorld::Body::FreeForm:
    {
        sf::Vector2f normal(manifold.x, manifold.y);
        move(normal * manifold.z);
        setVelocity(Util::Vector::reflect(getVelocity() * getFriction(), normal));
    }
    default: break;
    }
}

//-------------------------------------------
PlayerBehaviourWater::PlayerBehaviourWater(CollisionWorld::Body* b)
    : BodyBehaviour(b), m_sinkTime(1.5f), m_currentTime(0.f), m_splashed(false)
{
    if (!deleted())
    {
        Event e;
        e.type = Event::Type::Player;
        e.player.action = Event::PlayerEvent::HitWater;
        auto pos = b->getCentre();
        e.player.positionX = pos.x;
        e.player.positionY = pos.y;
        raiseEvent(e);
    }
}

void PlayerBehaviourWater::update(float dt)
{
    auto vel = getVelocity();
    vel.x = 0.f;
    vel.y *= 0.4f; //sink slowly - TODO raise event to stop jumping in this state?
    setVelocity(vel);

    m_currentTime += dt;
    if (m_currentTime > m_sinkTime 
        || (getFootSenseMask() & (CollisionWorld::Body::Type::Solid | CollisionWorld::Body::Block)))
        kill();
}

void PlayerBehaviourWater::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    if (!m_splashed && other->getType() == CollisionWorld::Body::Type::Water)
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
}