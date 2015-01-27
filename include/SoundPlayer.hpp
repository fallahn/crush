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

//plays a given sound effect

#ifndef SOUND_PLAYER_H_
#define SOUND_PLAYER_H_

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <map>
#include <list>

class Node;
class SoundPlayer final : private sf::NonCopyable
{
public:
    enum class AudioId
    {
        //game sounds
        PlayerJump,
        PlayerPickUp,
        PlayerDrop,
        PlayerGrab,
        PlayerRelease,
        PlayerDie,
        PlayerSpawn,
        ItemSpawn,
        ItemDespawn,
        ItemCollected,
        ItemExtraLife,
        ItemReverseControls,
        ItemSuperJumpSpeed,
        NpcDie,
        NpcJump,
        WaterSplash,
        BlockLand,
        BlockDrag,
        HatSpawn,
        HatCrush,
        HatLand,

        //UI sounds
        UIMove,
        UISelect
    };

    SoundPlayer();
    ~SoundPlayer() = default;

    void update();
    void play(AudioId id);
    void play(AudioId id, const sf::Vector2f& position, bool loop = false, Node* owner = nullptr);
    void stop(Node* owner);
    
    void setListenerPosition(const sf::Vector2f& position);
    sf::Vector2f getListenerPosition() const;
    void cacheSound(AudioId, const std::string&);

    static void setVolume(float volume);
    static float getVolume();

private:

    std::map<AudioId, sf::SoundBuffer> m_buffers;
    std::list<sf::Sound> m_sounds;
    std::list<std::pair<Node*, sf::Sound*>> m_loopedSounds;

    void flushSounds();
};

#endif //SOUND_PLAYER_H_