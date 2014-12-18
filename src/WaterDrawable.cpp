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

#include <WaterDrawable.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>

namespace
{
    const float tension = 2.9f;
    const float dampening = 0.009f;
    const float spread = 35.f;

    const sf::Uint16 pixelsPerColumn = 15u;
    const sf::Uint8 wavePasses = 4u;
}

WaterDrawable::WaterDrawable(const sf::Vector2f& size)
    : m_size        (size),
    m_lightColour   (0u, 204u, 255u, 140u),
    m_darkColour    (68u, 112u, 255u, 155u),
    m_vertices      (sf::TrianglesStrip)
{
    resize();
}

//public
void WaterDrawable::update(float dt)
{
    for (auto& c : m_columns)
        c.update(dt);

    auto columnCount = m_columns.size();
    auto endPoint = columnCount - 1u;

    std::vector<float> leftDeltas(columnCount);
    std::vector<float> rightDeltas(columnCount);

    for (auto i = 0u; i < wavePasses; ++i)
    {
        //calc the pull amount based on distance between surrounding points
        for (auto j = 0u; j < columnCount; ++j)
        {
            if (j > 0u) //skip testing first and last points
            {
                leftDeltas.push_back(((m_columns[j].height - m_columns[j - 1].height) * spread) * dt);
                m_columns[j - 1].speed += leftDeltas.back();
            }
            if (j < endPoint)
            {
                rightDeltas.push_back(((m_columns[j].height - m_columns[j + 1].height) * spread) * dt);
                m_columns[j + 1].speed += rightDeltas.back();
            }
        }

        //and apply 'pull' (we can't apply it right away else it affects the distance calc)
        for (auto j = 0u; j < columnCount; ++j)
        {
            if (j > 0u)
            {
                m_columns[j - 1].height += leftDeltas[j];
            }
            if (j < endPoint)
            {
                m_columns[j + 1].height += rightDeltas[j];
            }
        }
    }
}

void WaterDrawable::splash(float position, float speed)
{
    sf::Uint32 index = static_cast<sf::Uint32>(std::floor(position / pixelsPerColumn));
    assert(index < m_columns.size());

    m_columns[index].speed = speed;
    if (index > 0) m_columns[index - 1].speed = speed;
    if (index < m_columns.size() - 1) m_columns[index + 1].speed = speed;
}

void WaterDrawable::setSize(const sf::Vector2f& size)
{
    m_size = size;
    resize();
}

void WaterDrawable::setColours(const sf::Color& lightColour, const sf::Color& darkColour)
{
    m_lightColour = lightColour;
    m_darkColour = darkColour;
}

//private
void WaterDrawable::resize()
{
    m_columns.clear();
    auto count = static_cast<sf::Uint32>(std::ceil(m_size.x / pixelsPerColumn)) + 1u;
    for (auto i = 0u; i < count; ++i)
        m_columns.emplace_back();
}

void WaterDrawable::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    //rebuild vert array - TODO flag rebuild only when necessary
    m_vertices.clear();
    for (auto i = 0u; i < m_columns.size(); ++i)
    {
        auto offset = std::min(static_cast<float>(i * pixelsPerColumn), m_size.x);

        m_vertices.append(sf::Vertex({ offset, m_size.y }, m_darkColour));
        m_vertices.append(sf::Vertex({ offset, m_columns[i].height }, m_lightColour));
    }
    
    rt.draw(m_vertices, states);
}

WaterDrawable::Column::Column()
    : targetHeight  (0.f),
    height          (0.f),
    speed           (0.f){}

void WaterDrawable::Column::update(float dt)
{
    float amount = targetHeight - height;
    speed += (amount * tension) - (speed * dampening);
    height += speed * dt;
}