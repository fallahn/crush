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

#include <AIController.hpp>
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
            auto rf = Util::Random::value(600.f, 1000.f);
            rn = Util::Random::value(0, 1);
            if (rn == 1) rf = -rf;

            assert(n.getCollisionBody());
            n.getCollisionBody()->applyForce({ rf, 0.f });
        }
    }
}

AIController::AIController(CommandStack& c)
    : m_commandStack    (c),
    m_randTime          (10.f){}

void AIController::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Despawn:
        //spawn new NPC if needed
        break;
    case game::Event::Player:
        switch (evt.player.action)
        {
        case game::Event::PlayerEvent::Landed:
        {
            //get player position and point an npc at it
            sf::Vector2f pos(evt.player.positionX, evt.player.positionY);
            Command c;
            c.categoryMask = Category::Enemy;
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

void AIController::update(float dt)
{
    if (m_clock.getElapsedTime().asSeconds() > m_randTime)
    {
        m_randTime = Util::Random::value(3.f, 5.f);
        m_clock.restart();

        Command c;
        c.action = nudge;
        c.categoryMask |= Category::Enemy;
        m_commandStack.push(c);
    }
}