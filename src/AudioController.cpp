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

#include <AudioController.hpp>
#include <Node.hpp>
#include <Util.hpp>
#include <FileSystem.hpp>

#include <SFML/Audio/Listener.hpp>

#include <cmath>
#include <iostream>

AudioController::AudioController()
    : m_randomCount (0),
    m_randomTime    (2.f)
{
    //preload sounds
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerJump, "res/sound/fx/player_jump.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerPickUp, "res/sound/fx/player_pickup.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerDrop, "res/sound/fx/player_drop.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerGrab, "res/sound/fx/player_grab.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerRelease, "res/sound/fx/player_release.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerDie, "res/sound/fx/player_die.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::PlayerSpawn, "res/sound/fx/player_spawn.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::ItemSpawn, "res/sound/fx/item_spawn.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::ItemDespawn, "res/sound/fx/item_despawn.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::ItemExtraLife, "res/sound/fx/item_extra_life.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::ItemReverseControls, "res/sound/fx/item_reverse_controls.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::ItemSuperJumpSpeed, "res/sound/fx/item_superjump_superspeed.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::NpcDie, "res/sound/fx/npc_die.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::NpcJump, "res/sound/fx/npc_jump.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::WaterSplash, "res/sound/fx/hit_water.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::BlockLand, "res/sound/fx/block_drop.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::BlockDrag, "res/sound/fx/block_drag.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::HatCrush, "res/sound/fx/hat_crush.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::HatSpawn, "res/sound/fx/hat_spawn.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::HatLand, "res/sound/fx/hat_land.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::KillStreak, "res/sound/fx/killstreak.wav");

    m_soundPlayer.cacheSound(SoundPlayer::AudioId::Bat01, "res/sound/fx/bat01.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::Bat02, "res/sound/fx/bat02.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::Bird01, "res/sound/fx/bird01.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::Bird02, "res/sound/fx/bird02.wav");

    sf::Listener::setDirection(0.f, 0.f, -1.f);
    m_soundPlayer.setListenerPosition({ 960.f, 540.f }); //set to centre of world for now
}

//public
void AudioController::update()
{
    //spawn random ambience etc
    if (m_randomCount > 0 &&
     m_randomClock.getElapsedTime().asSeconds() > m_randomTime)
    {
        const float x = static_cast<float>(Util::Random::value(0, 1920));
        const float y = static_cast<float>(Util::Random::value(0, 1080));
        const float z = static_cast<float>(Util::Random::value(-1000, 1800));

        auto randStart = static_cast<int>(SoundPlayer::AudioId::Rand01);
        auto sound = static_cast<SoundPlayer::AudioId>(Util::Random::value(randStart, randStart + m_randomCount));
        //m_soundPlayer.play(sound, { x, y, z });

        m_randomTime = static_cast<float>(Util::Random::value(4, 10));
        m_randomClock.restart();
    }
    m_soundPlayer.update();
}

void AudioController::onNotify(Subject& s, const Event& e)
{
    switch (e.type)
    {
    case Event::Type::Player:
        switch (e.player.action)
        {
        case Event::PlayerEvent::Jumped:
            m_soundPlayer.play(SoundPlayer::AudioId::PlayerJump, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::PickedUp:
        case Event::PlayerEvent::GotHat:
            m_soundPlayer.play(SoundPlayer::AudioId::PlayerPickUp, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::Dropped:
            m_soundPlayer.play(SoundPlayer::AudioId::PlayerDrop, { e.player.positionX, e.player.positionY });
            std::cerr << "playdrop" << std::endl;
            break;
        case Event::PlayerEvent::Grabbed:
            m_soundPlayer.play(SoundPlayer::AudioId::PlayerGrab, { e.player.positionX, e.player.positionY });
            break;
        //case Event::PlayerEvent::Released:
        //    play(AudioId::PlayerRelease, { e.player.positionX, e.player.positionY });
        //    break;
        case Event::PlayerEvent::Died:
            m_soundPlayer.play(SoundPlayer::AudioId::PlayerDie, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::HitWater:
            m_soundPlayer.play(SoundPlayer::AudioId::WaterSplash, { e.player.positionX, e.player.positionY });
            break;
        case Event::PlayerEvent::GotItem:
            switch (e.player.item)
            {
            case Event::PlayerEvent::ExtraLife:
                m_soundPlayer.play(SoundPlayer::AudioId::ItemExtraLife, { e.player.positionX, e.player.positionY });
                break;
            case Event::PlayerEvent::ExtraSpeed:
            case Event::PlayerEvent::JumpIncrease:
                m_soundPlayer.play(SoundPlayer::AudioId::ItemSuperJumpSpeed, { e.player.positionX, e.player.positionY });
                break;
            case Event::PlayerEvent::ReverseControls:
                m_soundPlayer.play(SoundPlayer::AudioId::ItemReverseControls, { e.player.positionX, e.player.positionY });
                break;
            default: break;
            }
            break;
        case Event::PlayerEvent::KillStreak:
            m_soundPlayer.play(SoundPlayer::AudioId::KillStreak);
            break;
        default: break;
        }
        break;
    case Event::Type::Node:
        switch (e.node.action)
        {
        case Event::NodeEvent::Despawn:
            switch (e.node.type)
            {
            case Category::Item:
                m_soundPlayer.play(SoundPlayer::AudioId::ItemDespawn, { e.node.positionX, e.node.positionY });
                break;
            case Category::Npc:
                m_soundPlayer.play(SoundPlayer::AudioId::NpcDie, { e.node.positionX, e.node.positionY });
                break;
            case Category::HatCarried:
            case Category::HatDropped:
                m_soundPlayer.play(SoundPlayer::AudioId::HatCrush, { e.node.positionX, e.node.positionY });
                break;
            default: break;
            }
            break;
        case Event::NodeEvent::Spawn:
            switch (e.node.type)
            {
            case Category::Item:
                m_soundPlayer.play(SoundPlayer::AudioId::ItemSpawn, { e.node.positionX, e.node.positionY });
                break;
            case Category::PlayerOne:
            case Category::PlayerTwo:
                m_soundPlayer.play(SoundPlayer::AudioId::PlayerSpawn, { e.node.positionX, e.node.positionY });
                break;
            case Category::HatDropped:
                m_soundPlayer.play(SoundPlayer::AudioId::HatSpawn, { e.node.positionX, e.node.positionY });
                break;
            case Category::Bat:
                //m_soundPlayer.play((Util::Random::value(0, 1) == 0) ? SoundPlayer::AudioId::Bat01 : SoundPlayer::AudioId::Bat02, {}, false, static_cast<Node*>(&s));
                break;
            case Category::Bird:
                m_soundPlayer.play((Util::Random::value(0, 1) == 0) ? SoundPlayer::AudioId::Bird01 : SoundPlayer::AudioId::Bird02, {}, false, static_cast<Node*>(&s));
                break;
            default: break;
            }
            break;
        /*case Event::NodeEvent::HitWater:
            play(AudioId::WaterSplash, { e.node.positionX, e.node.positionY });
            break;*/
        }
        break;
    case Event::Type::Npc:
        switch (e.npc.action)
        {
        case Event::NpcEvent::HitWater:
            m_soundPlayer.play(SoundPlayer::AudioId::WaterSplash, { e.npc.positionX, e.npc.positionY });
            break;
        case Event::NpcEvent::Jumped:
            m_soundPlayer.play(SoundPlayer::AudioId::NpcJump, { e.npc.positionX, e.npc.positionY });
            break;
        default: break;
        }
        break;
    case Event::Type::Block:
        switch (e.block.action)
        {
        case Event::BlockEvent::HitWater:
            m_soundPlayer.play(SoundPlayer::AudioId::WaterSplash, { e.block.positionX, e.block.positionY });
            break;
        case Event::BlockEvent::HitGround:
            m_soundPlayer.play(SoundPlayer::AudioId::BlockLand, { e.block.positionX, e.block.positionY });
            break;
        case Event::BlockEvent::DragStart:
            {
                Node* node = static_cast<Node*>(&s);
                m_soundPlayer.play(SoundPlayer::AudioId::BlockDrag, { e.block.positionX, e.block.positionY }, true, node);   
            }
            break;
        case Event::BlockEvent::DragEnd:
        {
            Node* node = static_cast<Node*>(&s);
            m_soundPlayer.stop(node);
        }
            break;
        default: break;
        }
        break;
    case Event::Type::Hat:
        switch (e.hat.action)
        {
        case Event::HatEvent::HitGround:
            m_soundPlayer.play(SoundPlayer::AudioId::HatLand, { e.hat.positionX, e.hat.positionY });
            break;
        case Event::HatEvent::HitWater:
            m_soundPlayer.play(SoundPlayer::AudioId::WaterSplash, { e.hat.positionX, e.hat.positionY });
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void AudioController::loadTheme(const std::string& theme)
{
    std::string path = "res/sound/themes/" + theme + "/random";
    auto result = FileSystem::listFiles(path);

    auto randStart = static_cast<int>(SoundPlayer::AudioId::Rand01);
    auto count = std::min(static_cast<int>(SoundPlayer::AudioId::Rand09) - randStart, static_cast<int>(result.size()));
    for (auto i = 0; i < count; ++i)
    {
        m_soundPlayer.cacheSound(static_cast<SoundPlayer::AudioId>(randStart + i), path + "/" + result[i]);
    }

    m_randomCount = (count > 0) ? count - 1 : 0;
}

//private