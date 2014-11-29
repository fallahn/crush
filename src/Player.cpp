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

namespace
{
    const float joyDeadZone = 25.f;
    
    const float maxMoveForce = 220.f;
    const float jumpForce = 1900.f;

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
    joyButtonJump   (0u){}

Player::Player(CommandStack& cs, Category::Type type)
    : m_moveForce   (0.f),
    m_commandStack  (cs),
    m_id            (type),
    m_joyId         (0u),
    m_buttonMask    (0u),
    m_canSpawn      (true)
{
    assert(type == Category::PlayerOne || type == Category::PlayerTwo);
    if (type == Category::PlayerTwo)
    {
        m_joyId = 1u;
        m_keyBinds.left = sf::Keyboard::Left;
        m_keyBinds.right = sf::Keyboard::Right;
        m_keyBinds.jump = sf::Keyboard::Up;
    }
}

//public
void Player::update(float dt)
{
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
        };
        c.categoryMask |= m_id;
        m_commandStack.push(c);
    }

    //check for jump / grab
    if (sf::Keyboard::isKeyPressed(m_keyBinds.jump)
        || sf::Joystick::isButtonPressed(m_joyId, m_keyBinds.joyButtonJump))
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonJump)) == 0)
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
    case game::Event::Despawn:
        if (evt.despawn.type == m_id)
        {
            //oh noes, we died!
            m_canSpawn = true;
        }
        break;
    default: break;
    }
}

//private