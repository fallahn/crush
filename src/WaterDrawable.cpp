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

#include <WaterDrawable.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <cassert>

namespace
{
    const float tension = 2.9f;
    const float dampening = 0.009f;
    const float spread = 35.f;

    const sf::Uint16 pixelsPerColumn = 15u;
    const sf::Uint8 wavePasses = 4u;

    const std::vector<float> waveTable = { 0.f, 0.19f, 0.38f, 0.56f, 0.71f, 0.84f, 0.93f, 0.98f, 0.99f, 0.97f, 0.9f, 0.8f, 0.67f, 0.51f, 0.33f, 0.14f,
                                            0.f, -0.38f, -0.71f, -0.93f, -0.99f, -0.9f, -0.67f, -0.33f };

}

WaterDrawable::WaterDrawable(sf::Texture& normalMap, sf::Shader& shader, const sf::Vector2f& size)
    : m_size        (size),
    m_lightColour   (96u, 172u, 222u, 190u),//(64u, 72u, 45u, 130u),
    m_darkColour    (40u, 14u, 34u, 205u),
    m_vertices      (sf::TrianglesStrip),
    m_normalTexture (normalMap),
    m_texHeight     (static_cast<float>(m_normalTexture.getSize().y)),
    m_shader        (&shader),
    m_waveIndex     (0u),
    m_waveTime      (0.f)
{
    resize();

    m_normalTexture.setRepeated(true);
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

    //keep the surface moving
    m_waveIndex = (m_waveIndex + 1) % waveTable.size();
    m_columns[0].height = waveTable[m_waveIndex] * Util::Random::value(1.2f, 2.4f);
    m_columns.back().height = -m_columns[0].height;

    //update time to send to shader
    m_waveTime += (dt * 0.1f);
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

        m_vertices.append(sf::Vertex({ offset, m_size.y }, m_darkColour, { offset, m_texHeight }));
        m_vertices.append(sf::Vertex({ offset, m_columns[i].height }, m_lightColour, { offset, 0.f }));
    }
    
    m_shader->setParameter("u_normalMap", sf::Shader::CurrentTexture); //need to do this so tex coords are correct
    m_shader->setParameter("u_inverseWorldViewMatrix", states.transform.getInverse());
    m_shader->setParameter("u_textureOffset", m_waveTime);

    states.shader = m_shader;
    states.texture = &m_normalTexture;
    //states.blendMode = sf::BlendMultiply;
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