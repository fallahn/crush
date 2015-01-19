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

//responsible for parsing input and assigning the correct commands to the relevant player node

#ifndef PLAYER_H_
#define PLAYER_H_

#include <CommandStack.hpp>
#include <Observer.hpp>
#include <Resource.hpp>
#include <AnimatedSprite.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Clock.hpp>

#include <functional>

class Player final : public Observer// : private sf::NonCopyable
{
public:
    struct Keys
    {
        Keys();
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Keyboard::Key jump;
        sf::Keyboard::Key grab;
        sf::Keyboard::Key pickUp;
        sf::Uint8 joyButtonJump;
        sf::Uint8 joyButtonGrab;
        sf::Uint8 joyButtonPickUp;
    };

    Player(CommandStack& commandStack, Category::Type type, TextureResource& tr, sf::Shader& shader);
    ~Player() = default;

    void update(float dt);
    void handleEvent(const sf::Event& evt);

    void setKeyBinds(Keys keys);

    Category::Type getType() const;
    bool canSpawn() const;
    void setSpawnable(bool spawnable);

    void onNotify(Subject& s, const Event& evt) override;

    void setSpawnFunction(std::function<void(const sf::Vector2f&, Player&)>& func);
    void setSpawnPosition(const sf::Vector2f& position);

    sf::Drawable* getSprite();
    const sf::Vector2f& getSize() const;

private:
    float m_moveForce;
    float m_jumpForce;

    CommandStack& m_commandStack;
    Category::Type m_id, m_grabId, m_lastTouchId, m_carryId;
    sf::Uint8 m_joyId;

    Keys m_keyBinds;
    sf::Uint32 m_buttonMask;

    sf::Uint32 m_activeItems;
    float m_itemDuration;

    bool m_canSpawn;
    bool m_enabled;
    bool m_leftFacing;
    bool m_lastFacing;
    bool m_carryingBlock;
    sf::Vector2f m_carryVector; //position of block relative to player when carrying
    sf::Vector2f m_grabVector; //point from centre of body to detect grabbing range

    std::function<void(const sf::Vector2f&, Player&)> spawn;
    sf::Clock m_spawnClock;
    sf::Vector2f m_spawnPosition;
    sf::Vector2f m_currentPosition;
    sf::Vector2f m_size;

    sf::Texture m_normalTexture;
    AnimatedSprite m_sprite;
    AnimatedSprite m_powerupSprite;

    bool m_flashSprite;
    bool m_hasHat;

    void enable();
    void setSize(const sf::Vector2f& size);

    void doMovement(float dt);
    void doJump();
    void doGrab();
    void doRelease();
    void doPickUp();
    void doDrop(bool raiseEvent = true);
    void dropHat();
};

#endif //PLAYER_H_