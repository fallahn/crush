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

//sends commands to npcs and reacts to npc events
#ifndef NPC_CONTROLLER_H_
#define NPC_CONTROLLER_H_

#include <Observer.hpp>
#include <CommandStack.hpp>
#include <AnimatedSprite.hpp>
#include <Resource.hpp>
#include <ShaderResource.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>
#include <list>

class NpcController final : private sf::NonCopyable, public Observer
{
public:
    explicit NpcController(CommandStack& c, TextureResource& tr, ShaderResource& sr);
    ~NpcController() = default;

    void onNotify(Subject& s, const Event& evt) override;
    void update(float dt);

    void setSpawnFunction(std::function<void(const sf::Vector2f&, const sf::Vector2f&)>& func);
    void setNpcCount(sf::Uint8 count);

    sf::Drawable* getDrawable();

private:
    CommandStack& m_commandStack;

    sf::Clock m_movementClock;
    sf::Clock m_spawnClock;
    float m_randTime;

    bool m_enabled;
    sf::Uint8 m_npcSpawnCount; //how many to have on screen at a time

    TextureResource& m_textureResource;
    ShaderResource& m_shaderResource;
    std::list<AnimatedSprite> m_sprites;
    std::vector<sf::Drawable*> m_unusedSprites;


    std::function<void(const sf::Vector2f&, const sf::Vector2f&)> spawn;
};

#endif //NPC_CONTROLLER_H_