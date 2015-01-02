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
#include <Node.hpp>

#include <SFML/Audio/Listener.hpp>

#include <cmath>
#include <iostream>

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
    cacheSound(AudioId::PlayerJump, "res/sound/fx/player_jump.wav");
    cacheSound(AudioId::PlayerPickUp, "res/sound/fx/player_pickup.wav");
    cacheSound(AudioId::PlayerDrop, "res/sound/fx/player_drop.wav");
    cacheSound(AudioId::PlayerGrab, "res/sound/fx/player_grab.wav");
    cacheSound(AudioId::PlayerRelease, "res/sound/fx/player_release.wav");
    cacheSound(AudioId::PlayerDie, "res/sound/fx/player_die.wav");
    cacheSound(AudioId::ItemSpawn, "res/sound/fx/item_spawn.wav");
    cacheSound(AudioId::ItemDespawn, "res/sound/fx/item_despawn.wav");
    cacheSound(AudioId::ItemExtraLife, "res/sound/fx/item_extra_life.wav");
    cacheSound(AudioId::ItemReverseControls, "res/sound/fx/item_reverse_controls.wav");
    cacheSound(AudioId::ItemSuperJumpSpeed, "res/sound/fx/item_superjump_superspeed.wav");
    cacheSound(AudioId::NpcDie, "res/sound/fx/npc_die.wav");
    cacheSound(AudioId::NpcJump, "res/sound/fx/npc_jump.wav");
    cacheSound(AudioId::WaterSplash, "res/sound/fx/hit_water.wav");
    cacheSound(AudioId::BlockLand, "res/sound/fx/block_drop.wav");
    cacheSound(AudioId::BlockDrag, "res/sound/fx/block_drag.wav");

    sf::Listener::setDirection(0.f, 0.f, -1.f);
    setListenerPosition({ 960.f, 540.f }); //set to centre of world for now
}

//public
void AudioController::update()
{
    //TODO spawn random ambience etc

    //update playing sounds
    for (const auto& p : m_loopedSounds)
    {
        auto pos = p.first->getPosition();
        p.second->setPosition(pos.x, -pos.y, 0.f);
        auto cb = p.first->getCollisionBody();
        if (cb)
        {
            float speed = cb->getSpeed();
            //hmm magic consts ....
            speed = std::max(speed / 305000.f, 0.9f);
            p.second->setPitch(speed);
            //std::cerr << speed << std::endl;
        }
    }

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
        //case Event::PlayerEvent::Released:
        //    play(AudioId::PlayerRelease, { e.player.positionX, e.player.positionY });
        //    break;
        case Event::PlayerEvent::Died:
            play(AudioId::PlayerDie, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::HitWater:
            play(AudioId::WaterSplash, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::GotItem:
            switch (e.player.item)
            {
            case Event::PlayerEvent::ExtraLife:
                play(AudioId::ItemExtraLife, { e.player.positionX, e.player.positionY });
                break;
            case Event::PlayerEvent::ExtraSpeed:
            case Event::PlayerEvent::JumpIncrease:
                play(AudioId::ItemSuperJumpSpeed, { e.player.positionX, e.player.positionY });
                break;
            case Event::PlayerEvent::ReverseControls:
                play(AudioId::ItemReverseControls, { e.player.positionX, e.player.positionY });
                break;
            default: break;
            }
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
    case Event::Type::Block:
        switch (e.block.action)
        {
        case Event::BlockEvent::HitWater:
            play(AudioId::WaterSplash, { e.block.positionX, e.block.positionY });
            break;
        case Event::BlockEvent::HitGround:
            play(AudioId::BlockLand, { e.block.positionX, e.block.positionY });
            break;
        case Event::BlockEvent::DragStart:
            {
                auto& sound = play(AudioId::BlockDrag, { e.block.positionX, e.block.positionY }, true);
                Node* node = static_cast<Node*>(&s);
                m_loopedSounds.push_back(std::make_pair(node, &sound));
            }
            break;
        case Event::BlockEvent::DragEnd:
        {
            Node* node = static_cast<Node*>(&s);
            m_loopedSounds.remove_if([node](const std::pair<Node*, sf::Sound*>& p)
            {
                if (p.first == node)
                {
                    p.second->stop();
                    return true;
                }
                else return false;
            });
        }
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

sf::Sound& AudioController::play(AudioController::AudioId id, const sf::Vector2f& position, bool loop)
{
    m_sounds.emplace_back();

    auto& sound = m_sounds.back();
    sound.setBuffer(m_buffers[id]);
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(minDistance3D);
    sound.setLoop(loop);
    sound.play();

    return sound;
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

void AudioController::cacheSound(AudioId id, const std::string& path)
{
    m_buffers.insert(std::make_pair(id, sf::SoundBuffer()));
    m_buffers[id].loadFromFile(path);
}