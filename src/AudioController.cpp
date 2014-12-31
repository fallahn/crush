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

#include <AudioController.hpp>

#include <SFML/Audio/Listener.hpp>


namespace
{
    const float listenerDepth = 900.f; //TODO test these values
    const float attenuation = 1.f;
    const float minDistance2D = 700.f; //bear in mind listener won't be moving and we want to hear everything on screen
    const float minDistance3D = std::sqrt((minDistance2D * minDistance2D) + (listenerDepth * listenerDepth));
}

AudioController::AudioController()
{
    //preload sounds
    m_buffers.insert(std::make_pair(AudioId::PlayerJump, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerJump].loadFromFile("res/sound/fx/player_jump.wav");

    m_buffers.insert(std::make_pair(AudioId::PlayerPickUp, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerPickUp].loadFromFile("res/sound/fx/player_pickup.wav");

    m_buffers.insert(std::make_pair(AudioId::PlayerDrop, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerDrop].loadFromFile("res/sound/fx/player_drop.wav");

    m_buffers.insert(std::make_pair(AudioId::PlayerGrab, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerGrab].loadFromFile("res/sound/fx/player_grab.wav");

    m_buffers.insert(std::make_pair(AudioId::PlayerDrop, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerRelease].loadFromFile("res/sound/fx/player_release.wav");

    m_buffers.insert(std::make_pair(AudioId::PlayerDie, sf::SoundBuffer()));
    m_buffers[AudioId::PlayerDie].loadFromFile("res/sound/fx/player_die.wav");

    m_buffers.insert(std::make_pair(AudioId::ItemSpawn, sf::SoundBuffer()));
    m_buffers[AudioId::ItemSpawn].loadFromFile("res/sound/fx/item_spawn.wav");

    m_buffers.insert(std::make_pair(AudioId::ItemDespawn, sf::SoundBuffer()));
    m_buffers[AudioId::ItemDespawn].loadFromFile("res/sound/fx/item_despawn.wav");

    m_buffers.insert(std::make_pair(AudioId::NpcDie, sf::SoundBuffer()));
    m_buffers[AudioId::NpcDie].loadFromFile("res/sound/fx/npc_die.wav");

    m_buffers.insert(std::make_pair(AudioId::NpcJump, sf::SoundBuffer()));
    m_buffers[AudioId::NpcJump].loadFromFile("res/sound/fx/npc_jump.wav");

    m_buffers.insert(std::make_pair(AudioId::WaterSplash, sf::SoundBuffer()));
    m_buffers[AudioId::WaterSplash].loadFromFile("res/sound/fx/hit_water.wav");



    sf::Listener::setDirection(0.f, 0.f, -1.f);
    setListenerPosition({ 960.f, 540.f }); //set to centre of world for now
}

//public
void AudioController::update()
{
    //TODO spawn random ambience etc

    flushSounds();
}

void AudioController::onNotify(Subject& s, const Event& e)
{
    switch (e.type)
    {
    case Event::Type::Player:
        switch (e.player.action)
        {
        case Event::PlayerEvent::Jumped:
            play(AudioId::PlayerJump, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::PickedUp:
            play(AudioId::PlayerPickUp, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::Dropped:
            play(AudioId::PlayerDrop, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::Grabbed:
            play(AudioId::PlayerGrab, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::Released:
            play(AudioId::PlayerRelease, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::Died:
            play(AudioId::PlayerDie, { e.player.positionX, e.player.positionY });
            break;
        default: break;
        }
        break;
    case Event::Type::Node:
        if (e.node.action == Event::NodeEvent::Despawn)
        {
            switch (e.node.type)
            {
            case Category::Item:
                play(AudioId::ItemDespawn, { e.node.positionX, e.node.positionY });
                break;
            case Category::Npc:
                play(AudioId::NpcDie, { e.node.positionX, e.node.positionY });
                break;
            default: break;
            }
        }
        else if (e.node.action == Event::NodeEvent::Spawn)
        {
            switch (e.node.type)
            {
            case Category::Item:
                play(AudioId::ItemSpawn, { e.node.positionX, e.node.positionY });
                break;
            default: break;
            }
        }
        break;
    case Event::Type::Npc:
        switch (e.npc.action)
        {
        case Event::NpcEvent::HitWater:
            play(AudioId::WaterSplash, { e.npc.positionX, e.npc.positionY });
            break;
        case Event::NpcEvent::Jumped:
            play(AudioId::NpcJump, { e.npc.positionX, e.npc.positionY });
            break;
        default: break;
        }
        break;
    default: break;
    }
}

//private
void AudioController::play(AudioController::AudioId id)
{
    play(id, getListenerPosition());
}

void AudioController::play(AudioController::AudioId id, const sf::Vector2f& position)
{
    m_sounds.emplace_back();

    auto& sound = m_sounds.back();
    sound.setBuffer(m_buffers[id]);
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(minDistance3D);
    sound.play();
}

void AudioController::flushSounds()
{
    m_sounds.remove_if([](const sf::Sound& s){return (s.getStatus() == sf::Sound::Stopped); });
}

void AudioController::setListenerPosition(const sf::Vector2f& position)
{
    sf::Listener::setPosition(position.x, -position.y, listenerDepth);
}

sf::Vector2f AudioController::getListenerPosition() const
{
    auto pos = sf::Listener::getPosition();
    return{ pos.x, -pos.y };
}