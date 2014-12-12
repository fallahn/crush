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
    const float spawnTime = 1.f;
    const float joyDeadZone = 25.f;
    
    const float maxMoveForce = 200.f;
    const float jumpForce = 1650.f;

    const float friction = 0.86f;

    const sf::Vector2f pickupVec(0.f, 37.f); //TODO this needs to link to player height
    const sf::Vector2f grabVec(70.f, 0.f); //TODO this ought ot be tied to body size (just over half width)
}

Player::Keys::Keys()
    : left          (sf::Keyboard::A),
    right           (sf::Keyboard::D),
    jump            (sf::Keyboard::W),
    grab            (sf::Keyboard::Space),
    pickUp          (sf::Keyboard::S),
    joyButtonJump   (0u),
    joyButtonGrab   (1u),
    joyButtonPickUp (3u){}

Player::Player(CommandStack& cs, Category::Type type)
    : m_moveForce   (0.f),
    m_jumpForce     (jumpForce),
    m_commandStack  (cs),
    m_id            (type),
    m_grabId        (Category::GrabbedOne),
    m_lastTouchId   (Category::LastTouchedOne),
    m_carryId       (Category::CarriedOne),
    m_joyId         (0u),
    m_buttonMask    (0u),
    m_canSpawn      (true),
    m_enabled       (false),
    m_leftFacing    (false),
    m_carryingBlock (false),
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
        m_keyBinds.pickUp = sf::Keyboard::Down;

        m_leftFacing = true;
        m_spawnPosition = { 1680.f, 500.f };
        m_grabId = Category::GrabbedTwo;
        m_lastTouchId = Category::LastTouchedTwo;
        m_carryId = Category::CarriedTwo;
    }
}

//public
void Player::update(float dt)
{
    if(!m_enabled) return;

    doMovement();

    doJump();

    doGrab();

    doPickUp();

    //update the current position
    Command c;
    c.categoryMask |= m_id;
    c.action = [&](Node& n, float dt)
    {
        m_currentPosition = n.getCollisionBody()->getCentre();
    };
    m_commandStack.push(c);


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

            //let go of any blocks we were holding
            Command c;
            c.categoryMask |= m_grabId;
            c.action = [&](Node& n, float dt)
            {
                auto newCat = (n.getCategory() & ~m_grabId);
                newCat |= m_lastTouchId; //state player was last to touch
                n.setCategory(static_cast<Category::Type>(newCat));
            };
            m_commandStack.push(c);

            //and drop anything we were carrying
            doDrop();
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
            case game::Event::PlayerEvent::PickedUp:
            {
                //we picked up a block
                m_carryingBlock = true;
                Command c;
                c.categoryMask |= m_id;
                c.action = [](Node& n, float dt)
                {
                    assert(n.getCollisionBody());
                    n.getCollisionBody()->setFriction(friction * 0.75f);
                };
                m_commandStack.push(c);

                m_jumpForce = jumpForce * 0.75f;
            }
                break;
            default: break;
            }
        }
        break;
    case game::Event::Game:
        switch (evt.game.action)
        {
        case game::Event::GameEvent::PlayerOneEnable:
            if (m_id == Category::PlayerOne) enable();
            break;
        case game::Event::GameEvent::PlayerOneDisable:
            if (m_id == Category::PlayerOne) m_enabled = false;
            break;
        case game::Event::GameEvent::PlayerTwoEnable:
            if (m_id == Category::PlayerTwo) enable();
            break;
        case game::Event::GameEvent::PlayerTwoDisable:
            if (m_id == Category::PlayerTwo) m_enabled = false;
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void Player::setSpawnFunction(std::function<void(const sf::Vector2f&, Player&)>& func)
{
    spawn = func;
}

void Player::setSpawnPosition(const sf::Vector2f& position)
{
    m_spawnPosition = position;
}

void Player::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

//private
void Player::enable()
{
    m_enabled = true;
    spawn(m_spawnPosition, *this);
}

void Player::doMovement()
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
        m_leftFacing = (m_moveForce < 0.f);

        Command c;
        c.action = [&](Node& n, float dt)
        {
            assert(n.getCollisionBody());
            n.getCollisionBody()->applyForce({ m_moveForce, 0.f });
        };
        c.categoryMask |= m_id | m_grabId;
        m_commandStack.push(c);

        Command d;
        d.action = [&](Node& n, float dt)
        {
            assert(n.getCollisionBody());
            n.getCollisionBody()->setPosition(m_currentPosition + m_carryVector);
        };
        d.categoryMask |= m_carryId;
        m_commandStack.push(d);
    }
}

void Player::doJump()
{
    //check for jump
    if (sf::Keyboard::isKeyPressed(m_keyBinds.jump)
        || sf::Joystick::isButtonPressed(m_joyId, m_keyBinds.joyButtonJump))
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonJump)) == 0
            && (m_buttonMask & (1 << m_keyBinds.joyButtonGrab)) == 0)
        {
            Command c;
            c.action = [&](Node& n, float dt)
            {
                assert(n.getCollisionBody());
                n.getCollisionBody()->applyForce({ 0.f, -m_jumpForce });
            };
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

void Player::doGrab()
{
    if (m_carryingBlock) return; //can't drag when carrying

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
                auto point = (m_leftFacing) ? m_currentPosition - grabVec : m_currentPosition + grabVec;
                //and OR it's type with grabbed
                //TODO allow both players to grab same box?
                assert(n.getCollisionBody());
                if (n.getCollisionBody()->contains(point))
                {
                    auto cat = n.getCategory();
                    cat &= ~(Category::LastTouchedOne | Category::LastTouchedTwo); //make sure to remove any previous touches
                    n.setCategory(static_cast<Category::Type>(cat | m_grabId));
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
}

void Player::doPickUp()
{
    //check for picking up / dropping block
    if (sf::Keyboard::isKeyPressed(m_keyBinds.pickUp)
        || sf::Joystick::isButtonPressed(m_joyId, m_keyBinds.joyButtonPickUp))
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonPickUp)) == 0)
        {
            if (!m_carryingBlock)
            {
                //try picking up
                auto point = m_currentPosition + pickupVec;
                Command c;
                c.categoryMask |= Category::Block;
                c.action = [point, this](Node& n, float dt)
                {
                    auto cat = n.getCategory();
                    if (cat & (Category::GrabbedOne | Category::GrabbedTwo)) //don't pick up blocks being dragged
                        return;

                    assert(n.getCollisionBody());
                    if (n.getCollisionBody()->contains(point))
                    {
                        //pick it up
                        /*game::Event e;
                        e.type = game::Event::Node;
                        e.node.type = Category::Block;
                        e.node.action = game::Event::NodeEvent::Despawn;
                        auto pos = n.getCollisionBody()->getCentre();
                        e.node.positionX = pos.x;
                        e.node.positionY = pos.y;
                        n.raiseEvent(e);*/
                        
                        auto cat = n.getCategory();
                        cat |= (m_carryId | m_lastTouchId);
                        n.setCategory(static_cast<Category::Type>(cat));
                        
                        //let everyone know player picked up block
                        game::Event f;
                        f.type = game::Event::Player;
                        f.player.action = game::Event::PlayerEvent::PickedUp;
                        f.player.playerId = this->m_id;
                        f.player.positionX = this->m_currentPosition.x;
                        f.player.positionY = this->m_currentPosition.y;
                        n.raiseEvent(f);
                    }
                };
                m_commandStack.push(c);

                m_carryVector = (m_leftFacing) ?
                    sf::Vector2f(-(m_size.x * 1.5f), -(m_size.y * 0.75f)) :
                    sf::Vector2f(m_size.x * 0.5f, -(m_size.y * 0.75f));
            }
            else
            {
                //drop it
                doDrop();
            }
            m_buttonMask |= (1 << m_keyBinds.joyButtonPickUp);
        }
    }
    else
    {
        m_buttonMask &= ~(1 << m_keyBinds.joyButtonPickUp);
    }
}

void Player::doDrop()
{
    if (m_carryingBlock)
    {
        m_jumpForce = jumpForce;
        Command c;
        c.categoryMask |= m_id;
        c.action = [&](Node& n, float dt)
        {
            assert(n.getCollisionBody());
            n.getCollisionBody()->setFriction(friction); 
        };
        m_commandStack.push(c);

        //release block
        Command d;
        d.categoryMask |= m_carryId;
        d.action = [&](Node& n, float dt)
        {
            auto cat = n.getCategory();
            cat &= ~m_carryId;
            n.setCategory(static_cast<Category::Type>(cat));
        };
        m_commandStack.push(d);

        ////spawn block
        //game::Event e;
        //e.type = game::Event::Node;
        //e.node.action = game::Event::NodeEvent::Spawn;
        //e.node.positionX = (m_leftFacing) ?
        //    m_currentPosition.x - (m_size.x * 1.5f) :
        //    m_currentPosition.x + (m_size.x / 2.f);
        //e.node.positionY = m_currentPosition.y;// -(m_size.y / 2.f);
        //e.node.type = Category::Block;
        //notify(*this, e);

        m_carryingBlock = false;
    }
}