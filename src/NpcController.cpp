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

#include <NpcController.hpp>
#include <Node.hpp>
#include <Util.hpp>

#include <iostream>

namespace
{
    void nudge(Node& n, float dt)
    {
        auto rn = Util::Random::value(1, 4);
        if (rn == 1)
        {
            auto rf = Util::Random::value(400.f, 500.f);
            rn = Util::Random::value(0, 1);
            if (rn == 1) rf = -rf;

            assert(n.getCollisionBody());
            n.getCollisionBody()->applyForce({ rf, 0.f });
        }
    }

    const float npcSpawnTime = 1.f;

    const sf::Vector2i spriteSize(48, 56);
}

NpcController::NpcController(CommandStack& c, TextureResource& tr, ShaderResource& sr)
    : m_commandStack    (c),
    m_randTime          (10.f),
    m_enabled           (false),
    m_npcSpawnCount     (0u),
    m_textureResource   (tr),
    m_shaderResource    (sr){}

void NpcController::onNotify(Subject& s, const Event& evt)
{
    switch (evt.type)
    {
    case Event::Game:
        if (evt.game.action == Event::GameEvent::NpcEnable)
        {
            m_enabled = true;
        }
        else if (evt.game.action == Event::GameEvent::NpcDisable)
        {
            m_enabled = false;
        }
        break;
    case Event::Node:
        if (evt.node.type == Category::Npc && m_enabled)
        {
            switch (evt.node.action)
            {
            case Event::NodeEvent::Despawn:
            {
                m_npcSpawnCount++;
                m_spawnClock.restart();

                //remove sprite
                Node* n = static_cast<Node*>(&s);
                assert(n->getDrawable());
                //AnimatedSprite* sprite = static_cast<AnimatedSprite*>(n->getDrawable());
                m_unusedSprites.push_back(n->getDrawable());
                n->setDrawable(nullptr);
            }
                break;
            case Event::NodeEvent::Spawn:
                //spawn({ Util::Random::value(240.f, 1580.f), -40.f });          
                break;
            default: break;
            }
     }
        break;
    case Event::Player:
        switch (evt.player.action)
        {
        case Event::PlayerEvent::Landed:
        {
            //get player position and point an npc at it
            //sf::Vector2f pos(evt.player.positionX, evt.player.positionY);
            //Command c;
            //c.categoryMask = Category::Npc;
            //c.action = [=](Node& n, float dt)
            //{
            //    assert(n.getCollisionBody());
            //    if (Util::Random::value(1, 2) == 1)
            //    {
            //        //if an npc is lower when the player lands the resulting force
            //        //will catapult it upwards, so we only apply horizontal force
            //        auto dest = (pos - n.getCollisionBody()->getCentre()).x * 1.5f;
            //        n.getCollisionBody()->applyForce({ dest, 0.f });
            //    }
            //};
            //m_commandStack.push(c);
        }
            break;
        default: break;
        }
        break;
    case Event::Npc:
    {
        //NPC actions
        Node* node = static_cast<Node*>(&s);
        assert(node->getDrawable());
        AnimatedSprite& as = *static_cast<AnimatedSprite*>(node->getDrawable());
        switch (evt.npc.action)
        {
        case Event::NpcEvent::HitWater:
            as.setScale(as.getScale().x, -1.f);
            as.setOrigin(as.getOrigin().x, static_cast<float>(as.getFrameSize().y));
            as.setFrameRate(10.f); //TODO fixy magic numbers
            break;
        case Event::NpcEvent::Jumped:
            as.setScale(as.getScale().x, 1.f);
            as.setOrigin(as.getOrigin().x, 0.f);
            as.setFrameRate(18.f);
            as.setPaused(true);
            break;
        case Event::NpcEvent::Landed:
            //TODO some sort of particle effect?
            break;
        case Event::NpcEvent::Started:
            as.setPaused(false);
            break;
        case Event::NpcEvent::Stopped:
            as.setPaused(true);
            break;
        case Event::NpcEvent::TurnedLeft:
            as.setScale(1.f, 1.f);
            as.setOrigin(0.f, 0.f);
            break;
        case Event::NpcEvent::TurnedRight:
            as.setScale(-1.f, 1.f);
            as.setOrigin(static_cast<float>(as.getFrameSize().x), 0.f);
            break;
        default: break;
        }
    }
        break;
    default: break;
    }
}

void NpcController::update(float dt)
{
    if (!m_enabled) return;

    //add some random movement
    /*if (m_movementClock.getElapsedTime().asSeconds() > m_randTime)
    {
        m_randTime = Util::Random::value(3.f, 5.f);
        m_movementClock.restart();

        Command c;
        c.action = nudge;
        c.categoryMask |= Category::Npc;
        m_commandStack.push(c);
    }*/

    //spawn if minimum number not met
    if (m_npcSpawnCount > 0u 
        && m_spawnClock.getElapsedTime().asSeconds() > npcSpawnTime)
    {
        m_npcSpawnCount--;
        m_spawnClock.restart();
        spawn({ Util::Random::value(300.f, 1200.f), -40.f }, static_cast<sf::Vector2f>(spriteSize));
    }

    //update animated sprite
    for (auto& s : m_sprites)
        s.update(dt);

}

void NpcController::setSpawnFunction(std::function<void(const sf::Vector2f&, const sf::Vector2f&)>& func)
{
    spawn = func;
}

void NpcController::setNpcCount(sf::Uint8 count)
{
    m_npcSpawnCount = count;
}

sf::Drawable* NpcController::getDrawable()
{
    if (m_unusedSprites.size())
    {
        auto s = m_unusedSprites.back();
        m_unusedSprites.pop_back();
        return s;
    }
    else
    {
        m_sprites.emplace_back(m_textureResource.get("res/textures/robot_diffuse.png"));
        AnimatedSprite* s = &m_sprites.back();
        s->setNormalMap(m_textureResource.get("res/textures/robot_normal.tga"));
        s->setShader(m_shaderResource.get(Shader::Type::NormalMapSpecular));
        s->setFrameCount(8u);
        s->setFrameSize(spriteSize);
        s->setFrameRate(18.f);
        s->setLooped(true);
        s->play();
        return static_cast<sf::Drawable*>(s);
    }
}