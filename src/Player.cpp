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

#include <Player.hpp>
#include <Node.hpp>

#include <cassert>
#include <iostream>

namespace
{
    const float spawnTime = 2.6f;
    const float joyDeadZone = 25.f;
    
    const float maxMoveForce = 200.f;
    const float jumpForce = 1650.f;

    const float friction = 0.86f;

    void jump(Node& n, float dt)
    {
        assert(n.getCollisionBody());
        n.getCollisionBody()->applyForce({ 0.f, -jumpForce });
    }
}

Player::Keys::Keys()
    : left          (sf::Keyboard::A),
    right           (sf::Keyboard::D),
    jump            (sf::Keyboard::W),
    grab            (sf::Keyboard::Space),
    joyButtonJump   (0u),
    joyButtonGrab   (1u){}

Player::Player(CommandStack& cs, Category::Type type)
    : m_moveForce   (0.f),
    m_commandStack  (cs),
    m_id            (type),
    m_grabId        (Category::GrabbedOne),
    m_lastTouchId   (Category::LastTouchedOne),
    m_joyId         (0u),
    m_buttonMask    (0u),
    m_canSpawn      (true),
    m_enabled       (false),
    m_leftFacing    (false),
    m_spawnPosition (80.f, 500.f)
{
    assert(type == Category::PlayerOne || type == Category::PlayerTwo);
    if (type == Category::PlayerTwo)
    {
        m_joyId = 1u;
        m_keyBinds.left = sf::Keyboard::Left;
        m_keyBinds.right = sf::Keyboard::Right;
        m_keyBinds.jump = sf::Keyboard::Up;
        m_keyBinds.grab = sf::Keyboard::RShift;

        m_leftFacing = true;
        m_spawnPosition = { 1680.f, 500.f };
        m_grabId = Category::GrabbedTwo;
        m_lastTouchId = Category::LastTouchedTwo;
    }
}

//public
void Player::update(float dt)
{
    if(!m_enabled) return;

    //calc movement
    m_moveForce = 0;
    if (sf::Joystick::isConnected(m_joyId))
    {
        auto axisPos = sf::Joystick::getAxisPosition(m_joyId, sf::Joystick::PovX);
        if (axisPos < -joyDeadZone || axisPos > joyDeadZone)
        {
            m_moveForce = maxMoveForce * (axisPos / 100.f);
        }
        axisPos = sf::Joystick::getAxisPosition(m_joyId, sf::Joystick::X);
        if (axisPos < -joyDeadZone || axisPos > joyDeadZone)
        {
            m_moveForce = maxMoveForce * (axisPos / 100.f);
        }
    }
    //else
    {
        if (sf::Keyboard::isKeyPressed(m_keyBinds.left))
            m_moveForce -= maxMoveForce;
        if (sf::Keyboard::isKeyPressed(m_keyBinds.right))
            m_moveForce += maxMoveForce;
    }
    
    if (m_moveForce != 0.f)
    {
        Command c;
        c.action = [&](Node& n, float dt)
        {
            assert(n.getCollisionBody());
            n.getCollisionBody()->applyForce({ m_moveForce, 0.f });
            m_currentPosition = n.getCollisionBody()->getCentre();
        };
        c.categoryMask |= m_id | m_grabId;
        m_commandStack.push(c);

        m_leftFacing = (m_moveForce < 0.f);
    }

    //check for jump
    if (sf::Keyboard::isKeyPressed(m_keyBinds.jump)
        || sf::Joystick::isButtonPressed(m_joyId, m_keyBinds.joyButtonJump))
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonJump)) == 0
            && (m_buttonMask & (1 << m_keyBinds.joyButtonGrab)) == 0)
        {
            Command c;
            c.action = jump;
            c.categoryMask |= m_id;
            m_commandStack.push(c);
            m_buttonMask |= (1 << m_keyBinds.joyButtonJump);
        }
    }
    else
    {
        m_buttonMask &= ~(1 << m_keyBinds.joyButtonJump);
    }

    //check for grab
    if (sf::Keyboard::isKeyPressed(m_keyBinds.grab)
        || sf::Joystick::isButtonPressed(m_joyId, m_keyBinds.joyButtonGrab))
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonGrab)) == 0)
        {
            Command c;
            c.categoryMask |= Category::Block;
            c.action = [&](Node& n, float dt)
            {
                //ask node if it is in grabbing distance
                sf::Vector2f offset(70.f, 0.f); //TODO this ought ot be tied to body size (just over half width)
                auto point = (m_leftFacing) ? m_currentPosition - offset : m_currentPosition + offset;
                //and or it's type with grabbed
                //TODO allow both players to grab same box?
                assert(n.getCollisionBody());
                if (n.getCollisionBody()->contains(point))
                {
                    n.setCategory(static_cast<Category::Type>(n.getCategory() | m_grabId));
                }
            };

            m_commandStack.push(c);
            m_buttonMask |= (1 << m_keyBinds.joyButtonGrab);
        }
    }
    else
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonGrab)))
        {
            //send a command to all grabbed blocks to set their id as not grabbed
            Command c;
            c.categoryMask |= m_grabId;
            c.action = [&](Node& n, float dt)
            {
                auto newCat = (n.getCategory() & ~m_grabId);
                newCat |= m_lastTouchId; //state player was last to touch
                n.setCategory(static_cast<Category::Type>(newCat));
            };
            m_commandStack.push(c);
        }
        m_buttonMask &= ~(1 << m_keyBinds.joyButtonGrab);  
    }

    //TODO check life count
    //spawn a new player
    if (m_spawnClock.getElapsedTime().asSeconds() > spawnTime)
        spawn(m_spawnPosition, *this);
}

void Player::handleEvent(const sf::Event& evt)
{

}

void Player::setKeyBinds(Player::Keys keys)
{
    m_keyBinds = keys;
}

Category::Type Player::getType() const
{
    return m_id;
}

bool Player::canSpawn() const
{
    return m_canSpawn;
}

void Player::setSpawnable(bool spawnable)
{
    m_canSpawn = spawnable;
}

void Player::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Node:
        if (evt.node.type == m_id
            && evt.node.action == game::Event::NodeEvent::Despawn)
        {
            //oh noes, we died!
            m_canSpawn = true;
            m_spawnClock.restart();
        }
        break;
    case game::Event::Player:
        if (evt.player.playerId == m_id)
        {
            switch (evt.player.action)
            {
                //grabbing / releasing the blocks updates the players
                //friction so that they are slower when dragging
            case game::Event::PlayerEvent::Grabbed:
            {
                auto blockNode = dynamic_cast<Node*>(&s);
                assert(blockNode->getCollisionBody());
                float friction = blockNode->getCollisionBody()->getFriction();

                Command c;
                c.categoryMask |= m_id;
                c.action = [=](Node& n, float dt)
                {
                    assert(n.getCollisionBody());
                    n.getCollisionBody()->setFriction(friction);
                };
                m_commandStack.push(c);
            }
                break;
            case game::Event::PlayerEvent::Released:
            {
                Command c;
                c.categoryMask |= m_id;
                c.action = [](Node& n, float dt)
                {
                    assert(n.getCollisionBody());
                    n.getCollisionBody()->setFriction(friction);
                };
                m_commandStack.push(c);
            }
                break;
            }
        }
        break;
    default: break;
    }
}

void Player::setSpawnFunction(std::function<void(const sf::Vector2f&, Player&)>& func)
{
    spawn = func;
}

void Player::enable()
{
    m_enabled = true;
    spawn(m_spawnPosition, *this);
}

//private