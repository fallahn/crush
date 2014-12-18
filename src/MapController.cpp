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

#include <MapController.hpp>
#include <Map.hpp>
#include <Node.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include <map>

namespace
{
    const float spawnGapTime = 22.f;//time between spawns
}

MapController::MapController(CommandStack& cs)
    : m_commandStack    (cs),
    m_itemTime          (spawnGapTime),
    m_itemActive        (false)
{

}

//public
void MapController::update(float dt)
{
    m_itemTime -= dt;
    if (m_itemTime < 0)
    {
        //spawn a new item if none active
        if(!m_itemActive)
        {
            const auto& item = m_items.back();
            spawn(Category::Bonus, item.position, item.size);
            m_itemTime = item.lifeTime;
            m_items.pop_back();

            m_itemActive = true;
        }
        //or deactivate
        else
        {
            //technically this despawns all bonuses - but there should only ever be one
            //present in the scene at a time
            Command c;
            c.categoryMask |= Category::Bonus;
            c.action = [](Node& n, float dt)
            {
                game::Event evt;
                evt.type = game::Event::Node;
                evt.node.action = game::Event::NodeEvent::Despawn;
                evt.node.type = Category::Bonus;
                auto pos = n.getWorldPosition();
                evt.node.positionX = pos.x;
                evt.node.positionY = pos.y;
                n.raiseEvent(evt);
            };
            m_commandStack.push(c);

            m_itemTime = spawnGapTime;
            m_itemActive = false;
        }
    }
}

void MapController::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Node:
        switch (evt.node.action)
        {
        case game::Event::NodeEvent::Despawn:
            //check if item collected and do stuffs
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void MapController::setSpawnFunction(std::function<void(Category::Type, const sf::Vector2f&, const sf::Vector2f&)>& func)
{
    spawn = func;
}

void MapController::loadMap(const Map& map)
{
    const auto& nodes = map.getNodes();
    for (const auto& n : nodes)
    {
        if (n.type == Category::Bonus)
        {
            m_items.emplace_back(n.position, n.size, Util::Random::value(12.f, 19.f));
        }
        else
        {
            spawn(n.type, n.position, n.size);
        }

    }
    //shuffle item order
    std::random_shuffle(m_items.begin(), m_items.end(),
        [&](int i)
    {
        return Util::Random::value(0, m_items.size()) % i;
    });
}