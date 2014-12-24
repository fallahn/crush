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

#include <NpcBehaviour.hpp>
#include <Util.hpp>

namespace
{
    const float initialJumpSpeed = 700.f;
    const float damageMultiplier = 0.4f;
}

//-------------------------------------------
void NpcBehaviourAir::update(float dt)
{
    //reduce lateral velocity so sideways movement is minimal
    auto vel = getVelocity();
    vel.x *= 0.92f;
    setVelocity(vel);
}

void NpcBehaviourAir::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Water:
        //jump away
    {
        auto vel = getVelocity();
        if (vel.y >= 0) //only jump if moving down
        {
            setBehaviour<NpcBehaviourWater>();

            Event e;
            e.type = Event::Npc;
            e.npc.action = Event::NpcEvent::HitWater;
            raiseEvent(e);
        }
    }
    break;
    case CollisionWorld::Body::Type::Solid:
    case CollisionWorld::Body::Type::Block:
        //kill if block above and NPC is touching the ground
        if (manifold.y * manifold.z > 0 && getFootSenseCount() > 0)
        {
            //TODO: replace this with taking damage?
            kill();
            
            //raise event to say player killed us
            Event e;
            e.node.action = Event::NodeEvent::KilledNode;
            e.node.type = Category::Block;
            e.node.target = Category::Npc;

            if ((e.node.type & Category::LastTouchedOne) || (e.node.type & Category::GrabbedOne)) e.node.owner = Category::PlayerOne;
            else if ((e.node.type & Category::LastTouchedTwo) || (e.node.type & Category::GrabbedTwo)) e.node.owner = Category::PlayerTwo;
            else e.node.owner = Category::None;

            e.type = Event::Node;
            raiseEvent(e, other); //this should reference the other body as the sender not the NPC
            break; //if we should be dead then don't continue to resolve collision
        }

        move(sf::Vector2f( manifold.x, manifold.y ) * manifold.z);
    {   
        if (getFootSenseCount() > 0 && manifold.y * manifold.z < 0)
        {//set state at random
            if (Util::Random::value(0, 1))
            {
                setBehaviour<NpcBehaviourWalk>();
            }
            else
            {
                setBehaviour<NpcBehaviourGround>();
            }
        }
        else
        {
            //jump away from walls, or switch to ground sitting
            auto vel = getVelocity();
            //if (manifold.x != 0)
            vel.x = -vel.x;
            vel.y = 0.f;
            setVelocity(vel);

            Event e;
            e.type = Event::Npc;
            e.npc.action = (vel.x > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);
        }
    }
        break;
    case CollisionWorld::Body::Type::Player:
    {
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        auto vel = getVelocity();
        if (manifold.x != 0)
        {
            vel.x = -vel.x;
            Event e;
            e.type = Event::Npc;
            e.npc.action = (vel.x > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);
        }
        if (manifold.y != 0)
        {
            vel.y = -vel.y;
        }
        vel *= getFriction();
        setVelocity(vel);
    }
    break;
    case CollisionWorld::Body::Type::Npc:
        //steer away from other baddies
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        {
            auto vel = getVelocity();
            if (manifold.y != 0)
            {
                vel.y = -vel.y;
                vel.x = Util::Random::value(200.f, 400.f);
                if (Util::Random::value(0, 1) == 0)
                {
                    vel.x = -vel.x;
                }
            }
            if (manifold.x != 0)
            {
                vel.x = -vel.x;
            }
            setVelocity(vel);

            Event e;
            e.type = Event::Npc;
            e.npc.action = (vel.x > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);
        }
        break;

    default: break;
    }
}

//-------------------------------------------

NpcBehaviourGround::NpcBehaviourGround(CollisionWorld::Body* b)
    : BodyBehaviour     (b),
    m_changeDelay       (Util::Random::value(1.f, 2.f)),
    m_accumulatedTime   (0.f)
{
    setVelocity({});

    Event e;
    e.type = Event::Npc;
    e.npc.action = Event::NpcEvent::Landed;
    raiseEvent(e);

    e.npc.action = Event::NpcEvent::Stopped;
    raiseEvent(e);
}

void NpcBehaviourGround::update(float dt)
{
    auto vel = getVelocity();
    if(getFootSenseMask() & (CollisionWorld::Body::Type::Block | CollisionWorld::Body::Type::Solid)) vel.y = 0.f;
    vel.x *= getFriction();
       
    m_accumulatedTime += dt;
    if (m_accumulatedTime > m_changeDelay
        && getFootSenseCount() > 0)
    {
        //random between this and walking
        if (Util::Random::value(0, 1))
        {
            vel.x = (Util::Random::value(0, 1)) ? -400.f : 380.f;
            vel.y = -initialJumpSpeed;
            setBehaviour<NpcBehaviourAir>();

            Event e;
            e.type = Event::Npc;
            e.npc.action = (vel.x > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);

            e.npc.action = Event::NpcEvent::Jumped;
            raiseEvent(e);
        }
        else
        {
            setBehaviour<NpcBehaviourWalk>();
        }
    }

    setVelocity(vel);
}

void NpcBehaviourGround::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Water:
    {
        setBehaviour<NpcBehaviourWater>();

        Event e;
        e.type = Event::Npc;
        e.npc.action = Event::NpcEvent::HitWater;
        raiseEvent(e);
    }
    break;
    case CollisionWorld::Body::Type::Block:
        if (/*Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && */(manifold.x != 0.f || (manifold.y * manifold.z < 0))) //prevents shifting vertically down
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({});
            m_accumulatedTime = 0.f;
        }
        else if (manifold.y * manifold.z > 6)
        {
            //block is above, so crush
            kill();

            //raise event to say player killed us
            Event e;
            e.node.action = Event::NodeEvent::KilledNode;
            e.node.type = Category::Block;
            e.node.target = Category::Npc;

            int cat = other->getParentCategory();
            if (cat & (Category::LastTouchedOne | Category::GrabbedOne)) e.node.owner = Category::PlayerOne;
            else if (cat & (Category::LastTouchedTwo | Category::GrabbedTwo)) e.node.owner = Category::PlayerTwo;
            else e.node.owner = Category::None;

            e.type = Event::Node;
            raiseEvent(e, other);
        }
        {
            int cat = other->getParentCategory();
            if (cat & (Category::GrabbedOne | Category::GrabbedTwo | Category::LastTouchedOne | Category::LastTouchedTwo))
                damage(std::fabs(manifold.z * damageMultiplier), other);
        }
        break;
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        setVelocity({});
        break;
    case CollisionWorld::Body::Type::Player:
        break;

    case CollisionWorld::Body::Type::Npc:
        if (Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && manifold.x != 0.f) //prevents shifting vertically
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            setVelocity({});
        }
        break;
    default: break;
    }
}

//-------------------------------------------

NpcBehaviourWalk::NpcBehaviourWalk(CollisionWorld::Body* b)
    : BodyBehaviour     (b),
    m_changeDelay       (Util::Random::value(3.f, 5.f)),
    m_accumulatedTime   (0.f),
    m_moveForce         (Util::Random::value(28.f, 40.f)),
    m_applyGravity      (false)
{
    setVelocity({m_moveForce, 0.f});

    Event e;
    e.type = Event::Npc;
    e.npc.action= Event::NpcEvent::Landed;
    raiseEvent(e);

    e.npc.action = Event::NpcEvent::TurnedRight;
    raiseEvent(e);

    e.npc.action = Event::NpcEvent::Started;
    raiseEvent(e);
}

void NpcBehaviourWalk::update(float dt)
{
    if ((getFootSenseMask() & (CollisionWorld::Body::Type::Block | CollisionWorld::Body::Type::Solid)) == 0)
        m_applyGravity = true;

    auto vel = getVelocity();
    //only apply gravity when not on block / solid
    if(!m_applyGravity) vel.y = 0.f;
    vel.x += m_moveForce;
    vel.x *= getFriction();

    m_accumulatedTime += dt;
    if (m_accumulatedTime > m_changeDelay
        && getFootSenseCount() > 0)
    {
        //random between this and ground state
        if (Util::Random::value(0, 1) == 0)
        {
            vel.x = (Util::Random::value(0, 1)) ? -370.f : 390.f;
            vel.y = -initialJumpSpeed;
            setBehaviour<NpcBehaviourAir>();

            Event e;
            e.type = Event::Npc;
            e.npc.action = (vel.x > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);

            e.npc.action =  Event::NpcEvent::Jumped;
            raiseEvent(e);
        }
        else
        {
            setBehaviour<NpcBehaviourGround>();
        }
    }

    setVelocity(vel);
}

void NpcBehaviourWalk::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
{
    switch (other->getType())
    {
    case CollisionWorld::Body::Water:
    {
        setBehaviour<NpcBehaviourWater>();

        Event e;
        e.type = Event::Npc;
        e.npc.action = Event::NpcEvent::HitWater;
        raiseEvent(e);
    }
        break;
    case CollisionWorld::Body::Type::Block:
        if (/*Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && */(manifold.x != 0.f || (manifold.y * manifold.z < 0))) //prevents shifting vertically down
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            m_moveForce = -m_moveForce;
            setVelocity({});
            m_applyGravity = false;
            //m_accumulatedTime = 0.f;

            Event e;
            e.type = Event::Npc;
            e.npc.action = (m_moveForce > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);
        }
        else if (manifold.y * manifold.z > 5) //prevent dying when just clipping bottom by making sure there is some depth
        {
            //block is above, so crush
            kill();

            //raise event to say player killed us
            Event e;
            e.node.action = Event::NodeEvent::KilledNode;
            e.node.type = Category::Block;
            e.node.target = Category::Npc;

            int cat = other->getParentCategory();
            if (cat & (Category::LastTouchedOne | Category::GrabbedOne)) e.node.owner = Category::PlayerOne;
            else if (cat & (Category::LastTouchedTwo | Category::GrabbedTwo)) e.node.owner = Category::PlayerTwo;
            else e.node.owner = Category::None;

            e.type = Event::Node;
            raiseEvent(e, other);
        }
        {
            int cat = other->getParentCategory();
            if (cat & (Category::GrabbedOne | Category::GrabbedTwo | Category::LastTouchedOne | Category::LastTouchedTwo))
                damage(std::fabs(manifold.z * damageMultiplier), other);
        }
        break;
    case CollisionWorld::Body::Type::Solid:
        move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
        m_moveForce = -m_moveForce;
        setVelocity({});
        m_applyGravity = false;

        Event e;
        e.type = Event::Npc;
        e.npc.action = (m_moveForce > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
        raiseEvent(e);

        break;
    case CollisionWorld::Body::Type::Player:
        break;

    case CollisionWorld::Body::Type::Npc:
        if (Util::Vector::lengthSquared(getVelocity()) > 0.2f
            && manifold.x != 0.f) //prevents shifting vertically
        {
            move(sf::Vector2f(manifold.x, manifold.y) * manifold.z);
            m_moveForce = -m_moveForce;
            setVelocity({});

            Event e;
            e.type = Event::Npc;
            e.npc.action = (m_moveForce > 0) ? Event::NpcEvent::TurnedRight : Event::NpcEvent::TurnedLeft;
            raiseEvent(e);
        }
        break;
    default: break;
    }
}

//------------------------------------------

void NpcBehaviourWater::update(float dt)
{
    auto vel = getVelocity();
    vel.x = 0.f;
    vel.y *= getFriction();
    setVelocity(vel);

    m_currentTime += dt;
    if (m_currentTime > m_timeout
        || (getFootSenseMask() & (CollisionWorld::Body::Type::Solid || CollisionWorld::Body::Block)))
    {
        getBody()->setPosition({ Util::Random::value(300.f, 1600.f), -80.f });
        setBehaviour<NpcBehaviourAir>();

        Event e;
        e.type = Event::Npc;
        e.npc.action = Event::NpcEvent::Jumped;
        raiseEvent(e);
    }
}

void NpcBehaviourWater::resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other)
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