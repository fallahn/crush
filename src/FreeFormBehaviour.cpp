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

#include <iostream>

namespace
{
    const float maxSpeed = 900000.f;
    const float damageMultiplier = 1.46f;

    const float waterDuration = 24.f;
}

void FreeFormBehaviourAir::update(float dt)
{
    //hack to prevent as much tunnelling as possible
    const float limit = maxSpeed / getBody()->getSpeed();
    auto vel = getVelocity() * std::min(1.f, limit);
    setVelocity(vel * getFriction());

    //change state if we get carried
    if (getParentCategory() == Category::HatCarried)
    {
        setBehaviour<FreeFormBehaviourCarry>();
    }
}

void FreeFormBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Type::Water:
        setBehaviour<FreeFormBehaviourWater>();
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

            //use block splash sound
            Event e;
            e.type = Event::Hat;
            e.hat.action = Event::HatEvent::HitWater;
            auto pos = getBody()->getCentre();
            e.hat.positionX = pos.x;
            e.hat.positionY = pos.y;
            raiseEvent(e);
        }
        break;
    case CollisionWorld::Body::Block:
    case CollisionWorld::Body::Solid:
        if ((getFootSenseMask() & (CollisionWorld::Body::Solid | CollisionWorld::Body::Block))
            && getBody()->getSpeed() < 2600.f) //bounce a bit before switching to ground
        {
            setBehaviour<FreeFormBehaviourGround>();
            //std::cerr << getBody()->getSpeed() << ": switch to ground state" << std::endl;
        }

        //raise event
        {
            Event e;
            e.type = Event::Hat;
            e.hat.action = Event::HatEvent::HitGround;
            {
                auto pos = getBody()->getCentre();
                e.hat.positionX = pos.x;
                e.hat.positionY = pos.y;
            }
            raiseEvent(e);
        }

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
    {
        setBehaviour<FreeFormBehaviourAir>();
    }

    if (getFootSenseMask() & CollisionWorld::Body::Water)
    {
        m_expireTime += dt;
        if (m_expireTime >= waterDuration)
        {
            kill();
        }
    }

    //change state if we get carried
    if (getParentCategory() == Category::HatCarried)
    {
        setBehaviour<FreeFormBehaviourCarry>();
    }
}

void FreeFormBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Water:
        
        break;

    case CollisionWorld::Body::Block:
       
        if ((manifold.x != 0.f || (manifold.y * manifold.z < 0))) //prevents shifting vertically down
        {
            sf::Vector2f normal(manifold.x, manifold.y);
            move(normal * manifold.z);
            auto vel = getVelocity();
            vel = Util::Vector::reflect(vel, normal) * getFriction();
            vel += other->getVelocity();
            setVelocity(vel);
        }
        else if (manifold.y * manifold.z > 6)
        {
            //block is above, so crush
            kill();

            Event e;
            e.node.action = Event::NodeEvent::KilledNode;
            e.node.type = Category::Block;
            e.node.target = Category::HatDropped;

            /*int cat = other->getParentCategory();
            if (cat & (Category::LastTouchedOne | Category::GrabbedOne)) e.node.owner = Category::PlayerOne;
            else if (cat & (Category::LastTouchedTwo | Category::GrabbedTwo)) e.node.owner = Category::PlayerTwo;
            else */e.node.owner = Category::None; //TODO do we care who killed the hat?

            e.type = Event::Node;
            raiseEvent(e, other);
        }
        {
            int cat = other->getParentCategory();
            if (cat & (Category::GrabbedOne | Category::GrabbedTwo | Category::LastTouchedOne | Category::LastTouchedTwo))
                damage(std::fabs(manifold.z * damageMultiplier), other);
        }

        break;
    case CollisionWorld::Body::Solid:
        /*if (getBody()->getSpeed() > 2000.f)
        {
            Event e;
            e.type = Event::Block;
            e.block.action = Event::BlockEvent::HitGround;
            {
                auto pos = getBody()->getCentre();
                e.block.positionX = pos.x;
                e.block.positionY = pos.y;
            }
            raiseEvent(e);
        }*/
    default:
        sf::Vector2f normal(manifold.x, manifold.y);
        
        if (manifold.x != 0) //only move horizontally
            move(normal * manifold.z);

        auto vel = getVelocity();
        vel = Util::Vector::reflect(vel, normal) * getFriction();
        vel += other->getVelocity();
        setVelocity(vel);
        
        break;
    }
}

//--------------------------------------

void FreeFormBehaviourCarry::update(float dt)
{
    setVelocity({});

    //reset if no longer carried
    if (getParentCategory() != Category::HatCarried)
    {
        setBehaviour<FreeFormBehaviourAir>();
    }
}

void FreeFormBehaviourCarry::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Npc:
        ////kill teh hats
        //break;
    case CollisionWorld::Body::Block:
        //kill if from above
        if (manifold.y * manifold.z > 6)
        {
            kill();

            Event e;
            e.node.action = Event::NodeEvent::KilledNode;
            e.node.type = Category::Block;
            e.node.target = Category::HatDropped;
            e.node.owner = Category::None;

            e.type = Event::Node;
            raiseEvent(e, other);
        }
        break;
    default: break;
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
