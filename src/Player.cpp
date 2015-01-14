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

#include <Player.hpp>
#include <Node.hpp>
#include <Util.hpp>

#include <cassert>
#include <iostream>

namespace
{
    const float spawnTime = 1.f;
    const float joyDeadZone = 25.f;
    
    //large value because they are reduced by delta time
    const float maxMoveForce = 8200.f;
    const float jumpForce = 1180.f;
    const float friction = 0.83f;

    //block interaction
    const float pickupPadding = 3.2f;
    const float dragPadding = 0.52f; //multiple of block size
    const float pickupHeight = 14.f;
    const float carryForceReduction = 0.75f;

    //item modifiers
    const float itemDuration = 16.f;
    const float speedIncrease = 1.8f;
    const float jumpIncrease = 1.3f;

    //animation consts
    const float maxFrameRate = 12.f;
    const sf::Vector2i frameSize(41, 64);
    Animation idle(2, 2);
    Animation run(0, 5);
    Animation jump(6, 6);
    Animation fall(7, 7);

    const sf::Vector2f hatPosition(0.f, -29.f);
}

Player::Keys::Keys()
    : left          (sf::Keyboard::A),
    right           (sf::Keyboard::D),
    jump            (sf::Keyboard::W),
    grab            (sf::Keyboard::Space),
    pickUp          (sf::Keyboard::S),
    joyButtonJump   (0u),
    joyButtonGrab   (1u),
    joyButtonPickUp (2u){}

Player::Player(CommandStack& cs, Category::Type type, TextureResource& tr, sf::Shader& shader)
    : m_moveForce   (0.f),
    m_jumpForce     (jumpForce),
    m_commandStack  (cs),
    m_id            (type),
    m_grabId        (Category::GrabbedOne),
    m_lastTouchId   (Category::LastTouchedOne),
    m_carryId       (Category::CarriedOne),
    m_joyId         (0u),
    m_buttonMask    (0u),
    m_activeItems   (0u),
    m_itemDuration  (0.f),
    m_canSpawn      (true),
    m_enabled       (false),
    m_leftFacing    (false),
    m_lastFacing    (false),
    m_carryingBlock (false),
    m_spawnPosition (80.f, 500.f),
    m_flashSprite   (true),
    m_hasHat        (false)
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
        m_lastFacing = m_leftFacing;
        m_grabId = Category::GrabbedTwo;
        m_lastTouchId = Category::LastTouchedTwo;
        m_carryId = Category::CarriedTwo;

        m_sprite.setTexture(tr.get("res/textures/playerTwo_diffuse.png"));
        m_sprite.setScale(-1.f, 1.f);
        m_sprite.setOrigin(static_cast<float>(frameSize.x), 0.f); //uhhh iron this out at some point
    }
    else
    {
        m_sprite.setTexture(tr.get("res/textures/playerOne_diffuse.png"));
    }

    m_sprite.setNormalMap(tr.get("res/textures/player_normal.png"));
    m_sprite.setShader(shader);
    m_sprite.setFrameCount(8u);
    m_sprite.setFrameRate(maxFrameRate);
    m_sprite.setFrameSize(frameSize);
    m_sprite.setLooped(true);
    m_sprite.play(idle);

    setSize(static_cast<sf::Vector2f>(m_sprite.getFrameSize()));
}

//public
void Player::update(float dt)
{
    if(!m_enabled) return;

    doMovement(dt);

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

    //check active items for expiry
    if (m_activeItems)
    {
        m_itemDuration += dt;
        if (m_itemDuration > itemDuration)
        {
            m_activeItems = 0u;
            m_itemDuration = 0.f;
            m_jumpForce = jumpForce;
            std::cout << "item expired" << std::endl;
        }
    }

    //spawn a new player
    if (m_canSpawn && m_spawnClock.getElapsedTime().asSeconds() > spawnTime)
    {
        m_flashSprite = true;
        spawn(m_spawnPosition, *this);
    }

    //update sprite
    float fr = maxFrameRate / 2.f;
    m_sprite.setFrameRate(fr + (maxFrameRate * abs(m_moveForce / (maxMoveForce * dt))));
    m_sprite.update(dt);

    if (m_flashSprite)
    {
        m_sprite.setColour({
            //static_cast<sf::Uint8>(Util::Random::value(1, 255)),
            //static_cast<sf::Uint8>(Util::Random::value(1, 255)),
            //static_cast<sf::Uint8>(Util::Random::value(1, 255)),
            255u, 255u, 255u,
            static_cast<sf::Uint8>(Util::Random::value(1, 255)) });
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

void Player::onNotify(Subject& s, const Event& evt)
{
    switch (evt.type)
    {
    case Event::Node:
        if (evt.node.type == m_id)
        {    
            switch (evt.node.action)
            {
            case Event::NodeEvent::Despawn:
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

                //reset active powerups
                m_activeItems = 0u;
                m_jumpForce = jumpForce;

                //and drop anything we were carrying
                doDrop();
                doRelease();
                m_carryingBlock = false;
                dropHat();
                
            }
                    break;
            case Event::NodeEvent::InvincibilityExpired:
                m_flashSprite = false;
                m_sprite.setColour(sf::Color::White);
                break;
            default: break;
            }
        }
        else if (evt.node.type == Category::HatCarried)
        {
            m_hasHat = false;
        }
        break;
    case Event::Player:
        if (evt.player.playerId == m_id)
        {
            switch (evt.player.action)
            {
            case Event::PlayerEvent::Moved:
                m_sprite.play(run);
                break;
            case Event::PlayerEvent::StartedFalling:
                m_sprite.play(fall);
                break;
            case Event::PlayerEvent::Stopped:
            case Event::PlayerEvent::Landed:
                m_sprite.play(idle);

                //make sure friction is set when carrying block
                if (m_carryingBlock)
                {
                    Command d;
                    d.categoryMask |= m_id;
                    d.action = [&, this](Node& on, float dt)
                    {
                        assert(on.getCollisionBody());
                        on.getCollisionBody()->setFriction(friction * carryForceReduction);
                    };
                    m_commandStack.push(d);
                    m_jumpForce = jumpForce * carryForceReduction;
                }
                break;

                //grabbing / releasing the blocks updates the players
                //friction so that they are slower when dragging
            case Event::PlayerEvent::Grabbed:
            {
                if (m_canSpawn) break; //don't do anything, we're dead
                
                auto blockNode = dynamic_cast<Node*>(&s);
                assert(blockNode->getCollisionBody());
                auto blockBody = blockNode->getCollisionBody();
                float friction = blockBody->getFriction();

                Command c;
                c.categoryMask |= m_id;
                c.action = [=](Node& n, float dt)
                {
                    assert(n.getCollisionBody());
                    auto playerBody = n.getCollisionBody();
                    playerBody->setFriction(friction);
                    auto blockSize = blockBody->getSize();

                    sf::Vector2f pickup;
                    pickup.x = (m_leftFacing) ? -(m_size.x + blockSize.x) * dragPadding : (m_size.x + blockSize.x) * dragPadding;
                    pickup.y = playerBody->getSize().y - blockSize.y;
                    playerBody->addChild(blockBody, pickup);
                };
                m_commandStack.push(c);
            }
                break;
            case Event::PlayerEvent::Released:
            {
                Command c;
                c.categoryMask |= m_id;
                c.action = [&](Node& n, float dt)
                {
                    assert(n.getCollisionBody());
                    n.getCollisionBody()->setFriction(friction);
                    n.getCollisionBody()->removeChild(dynamic_cast<Node*>(&s)->getCollisionBody());
                };
                m_commandStack.push(c);

                doRelease();
            }
                break;
            case Event::PlayerEvent::PickedUp:
            {
                //we picked up a block
                
            }
                break;
            case Event::PlayerEvent::Dropped:
                //something made us drop the block
                doDrop();
                break;
            case Event::PlayerEvent::GotItem:
                switch (evt.player.item)
                {
                case Event::PlayerEvent::ExtraSpeed:
                    m_activeItems |= (1 << Event::PlayerEvent::ExtraSpeed);
                    std::cout << "increase player speed" << std::endl;
                    break;
                case Event::PlayerEvent::JumpIncrease:
                    m_activeItems |= (1 << Event::PlayerEvent::JumpIncrease);
                    m_jumpForce = jumpForce * jumpIncrease;
                    std::cout << "increase player jump" << std::endl;
                    break;
                case Event::PlayerEvent::ReverseControls:
                    m_activeItems |= (1 << Event::PlayerEvent::ReverseControls);
                    std::cout << "reverse player controls" << std::endl;
                    break;
                case Event::PlayerEvent::Attraction:
                    std::cout << "NPC attraction not yet implemented" << std::endl;
                    break;
                default: break;
                }
                break;
            default: break;
            }
        }
        break;
    case Event::Game:
        switch (evt.game.action)
        {
        case Event::GameEvent::PlayerOneEnable:
            if (m_id == Category::PlayerOne) enable();
            break;
        case Event::GameEvent::PlayerOneDisable:
            if (m_id == Category::PlayerOne) m_enabled = false;
            break;
        case Event::GameEvent::PlayerTwoEnable:
            if (m_id == Category::PlayerTwo) enable();
            break;
        case Event::GameEvent::PlayerTwoDisable:
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
    //this ought to size.x + object.x / 2
    //but how to get the size of objects?
    m_grabVector.x = (size.x + 64.f) / 2.f;
}

sf::Drawable* Player::getSprite()
{
    return static_cast<sf::Drawable*>(&m_sprite);
}

const sf::Vector2f& Player::getSize() const
{
    return m_size;
}

//private
void Player::enable()
{
    m_enabled = true;
    spawn(m_spawnPosition, *this);
}

void Player::doMovement(float dt)
{
    //calc movement
    m_moveForce = 0;
    if (sf::Joystick::isConnected(m_joyId))
    {
        auto axisPos = sf::Joystick::getAxisPosition(m_joyId, sf::Joystick::PovX);
        if (axisPos < -joyDeadZone || axisPos > joyDeadZone)
        {
            m_moveForce = maxMoveForce * (axisPos / 100.f) * dt;
        }
        axisPos = sf::Joystick::getAxisPosition(m_joyId, sf::Joystick::X);
        if (axisPos < -joyDeadZone || axisPos > joyDeadZone)
        {
            m_moveForce = maxMoveForce * (axisPos / 100.f) * dt;
        }
    }
    //else
    {
        if (sf::Keyboard::isKeyPressed(m_keyBinds.left))
            m_moveForce -= maxMoveForce * dt;
        if (sf::Keyboard::isKeyPressed(m_keyBinds.right))
            m_moveForce += maxMoveForce * dt;
    }

    //add speed increase if active
    if (m_activeItems & (1 << Event::PlayerEvent::ExtraSpeed))
        m_moveForce *= speedIncrease;
    //invert movement if reverse active
    if (m_activeItems & (1 << Event::PlayerEvent::ReverseControls))
        m_moveForce = -m_moveForce;

    if (m_moveForce != 0.f)
    {
        if ((m_buttonMask & (1 << m_keyBinds.joyButtonGrab)) == 0)
            m_leftFacing = (m_moveForce < 0.f); //only switch direction when not dragging

        bool flip = (m_leftFacing != m_lastFacing);

        Command c;
        c.action = [&, flip](Node& n, float dt)
        {
            assert(n.getCollisionBody());
            n.getCollisionBody()->applyForce({ m_moveForce, 0.f });

            //flip carried blocks
            if (m_carryingBlock && flip)
            {
                n.getCollisionBody()->flipChildren();
            }

        };
        c.categoryMask |= m_id | m_grabId;
        m_commandStack.push(c);

        m_lastFacing = m_leftFacing;

        //if changed direction update sprite
        if (flip)
        {
            if (m_leftFacing)
            {
                m_sprite.setScale(-1.f, 1.f);
                m_sprite.setOrigin(static_cast<float>(m_sprite.getFrameSize().x), 0.f);
            }
            else
            {
                m_sprite.setScale(1.f, 1.f);
                m_sprite.setOrigin(0.f, 0.f);
            }
        }
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

                Event e;
                e.type = Event::Type::Player;
                e.player.action = Event::PlayerEvent::Jumped;               
                e.player.positionX = m_currentPosition.x;
                e.player.positionY = m_currentPosition.y;
                n.raiseEvent(e);

            };
            c.categoryMask |= m_id;
            m_commandStack.push(c);
            m_buttonMask |= (1 << m_keyBinds.joyButtonJump);
        }

        //jump animation
        m_sprite.play(jump);
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
                auto point = (m_leftFacing) ? m_currentPosition - m_grabVector : m_currentPosition + m_grabVector;
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
            doRelease();
        }
        m_buttonMask &= ~(1 << m_keyBinds.joyButtonGrab);
    }
}

void Player::doRelease()
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
                m_carryVector = (m_leftFacing) ? //we add padding here to stop the player and block overlapping (and causing collision problems)
                    sf::Vector2f(-(m_grabVector.x + pickupPadding), 0.f) : //m_size.x
                    sf::Vector2f(m_grabVector.x + pickupPadding, 0.f);

                Command c;
                c.categoryMask |= Category::Block;
                c.action = [&](Node& n, float dt)
                {
                    auto point = (m_leftFacing) ? m_currentPosition - m_grabVector : m_currentPosition + m_grabVector;
                    
                    auto cat = n.getCategory();
                    if (cat & (Category::GrabbedOne | Category::GrabbedTwo | Category::CarriedOne | Category::CarriedTwo)) //don't pick up blocks being dragged
                        return;

                    assert(n.getCollisionBody());
                    if (n.getCollisionBody()->contains(point))
                    {
                        //pick it up                     
                        auto cat = n.getCategory();
                        //unset previous touches
                        cat &= ~(Category::LastTouchedOne | Category::LastTouchedTwo);
                        cat |= (m_carryId);
                        n.setCategory(static_cast<Category::Type>(cat));
                        
                        //let everyone know player picked up block
                        Event f;
                        f.type = Event::Player;
                        f.player.action = Event::PlayerEvent::PickedUp;
                        f.player.playerId = m_id;
                        f.player.positionX = m_currentPosition.x;
                        f.player.positionY = m_currentPosition.y;
                        n.raiseEvent(f);

                        //place a command to the player node to add this body as a child
                        Command d;
                        d.categoryMask |= m_id;
                        d.action = [&, this](Node& on, float dt)
                        {
                            assert(on.getCollisionBody());

                            this->m_carryVector.y = (on.getCollisionBody()->getSize().y - n.getCollisionBody()->getSize().y) - pickupHeight;                           
                            on.getCollisionBody()->addChild(n.getCollisionBody(), this->m_carryVector);
                            on.getCollisionBody()->setFriction(friction * carryForceReduction);
                        };
                        m_commandStack.push(d);

                        //should only become true if we manage to pick up block
                        m_carryingBlock = true;
                        m_jumpForce = jumpForce * carryForceReduction;
                    }
                };
                m_commandStack.push(c); 


                //look to see if we can pick up the hat
                Command e;
                e.categoryMask |= Category::HatDropped;
                e.action = [&, this](Node& n, float dt)
                {
                    auto point = (m_leftFacing) ? m_currentPosition - m_grabVector : m_currentPosition + m_grabVector;

                    assert(n.getCollisionBody());
                    if (n.getCollisionBody()->contains(point))
                    {
                        n.setCategory(Category::HatCarried);

                        //raise event to say we picked up hat
                        Event evt;
                        evt.type = Event::Player;
                        evt.player.action = Event::PlayerEvent::GotHat;
                        evt.player.playerId = m_id;
                        auto position = n.getWorldPosition();
                        evt.player.positionX = position.x;
                        evt.player.positionY = position.y;
                        n.raiseEvent(evt);

                        //raise a command for player node to attach hat body to player body
                        Command f;
                        f.categoryMask |= m_id;
                        f.action = [&, this](Node& on, float dt)
                        {
                            assert(on.getCollisionBody());
                            on.getCollisionBody()->addChild(n.getCollisionBody(), hatPosition);
                        };
                        m_commandStack.push(f);

                        m_hasHat = true;
                    }
                };
                m_commandStack.push(e);
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
        if (m_activeItems & (1 << Event::PlayerEvent::ExtraSpeed))
            m_jumpForce *= jumpIncrease;
        
        //release block
        Command c;
        c.categoryMask |= m_carryId;
        c.action = [&](Node& n, float dt)
        {
            auto cat = n.getCategory();
            cat &= ~m_carryId;
            cat |= m_lastTouchId;
            n.setCategory(static_cast<Category::Type>(cat));

            //let the world know block was dropped
            Event e;
            e.type = Event::Type::Player;
            e.player.action = Event::PlayerEvent::Dropped;
            e.player.positionX = m_currentPosition.x;
            e.player.positionY = m_currentPosition.y;
            n.raiseEvent(e);

            //command player to return friction
            //and remove child
            Command d;
            d.categoryMask |= m_id;
            d.action = [&](Node& on, float dt)
            {
                assert(on.getCollisionBody());
                on.getCollisionBody()->setFriction(friction);
                on.getCollisionBody()->removeChild(n.getCollisionBody());
            };
            m_commandStack.push(d);

        };
        m_commandStack.push(c);

        m_carryingBlock = false;
    }
}

void Player::dropHat()
{
    if (m_hasHat)
    {
        Command c;
        c.categoryMask |= Category::HatCarried;
        c.action = [this](Node& n, float dt)
        {
            n.setCategory(Category::HatDropped);

            //raise dropped event
            Event evt;
            evt.type = Event::Player;
            evt.player.action = Event::PlayerEvent::LostHat;
            evt.player.playerId = m_id;
            auto position = n.getWorldPosition();
            evt.player.positionX = position.x;
            evt.player.positionY = position.y;
            n.raiseEvent(evt);
        };
        m_commandStack.push(c);
        m_hasHat = false;
    }
}