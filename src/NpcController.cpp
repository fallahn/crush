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
}

NpcController::NpcController(CommandStack& c)
    : m_commandStack    (c),
    m_randTime          (10.f),
    m_enabled           (true),
    m_npcSpawnCount     (0u){}

void NpcController::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Game:
        if (evt.game.action == game::Event::GameEvent::NpcEnable)
        {
            m_enabled = true;
        }
        else if (evt.game.action == game::Event::GameEvent::NpcDisable)
        {
            m_enabled = false;
        }
        break;
    case game::Event::Node:
        if (evt.node.type == Category::Npc && m_enabled)
        {
            switch (evt.node.action)
            {
            case game::Event::NodeEvent::Despawn:
                m_npcSpawnCount++;
                m_spawnClock.restart();
                std::cout << "NPC Spawn count: " << (int)m_npcSpawnCount << std::endl;
                break;
            case game::Event::NodeEvent::Spawn:
                //spawn({ Util::Random::value(240.f, 1580.f), -40.f });          
                break;
            default: break;
            }
     }
        break;
    case game::Event::Player:
        switch (evt.player.action)
        {
        case game::Event::PlayerEvent::Landed:
        {
            //get player position and point an npc at it
            sf::Vector2f pos(evt.player.positionX, evt.player.positionY);
            Command c;
            c.categoryMask = Category::Npc;
            c.action = [=](Node& n, float dt)
            {
                assert(n.getCollisionBody());
                if (Util::Random::value(1, 2) == 1)
                {
                    //if an npc is lower when the player lands the resulting force
                    //will catapult it upwards, so we only apply horizontal force
                    auto dest = (pos - n.getCollisionBody()->getCentre()).x * 1.5f;
                    n.getCollisionBody()->applyForce({ dest, 0.f });
                }
            };
            m_commandStack.push(c);
        }
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void NpcController::update(float dt)
{
    if (!m_enabled) return;

    if (m_movementClock.getElapsedTime().asSeconds() > m_randTime)
    {
        m_randTime = Util::Random::value(3.f, 5.f);
        m_movementClock.restart();

        Command c;
        c.action = nudge;
        c.categoryMask |= Category::Npc;
        m_commandStack.push(c);
    }

    if (m_npcSpawnCount > 0u 
        && m_spawnClock.getElapsedTime().asSeconds() > npcSpawnTime)
    {
        m_npcSpawnCount--;
        m_spawnClock.restart();
        spawn({ Util::Random::value(300.f, 1200.f), -40.f });

        std::cout << "NPC Spawn count: " << (int)m_npcSpawnCount << std::endl;
    }
}

void NpcController::setSpawnFunction(std::function<void(const sf::Vector2f&)>& func)
{
    spawn = func;
}

void NpcController::setNpcCount(sf::Uint8 count)
{
    m_npcSpawnCount = count;
}