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

#include <ScoreBar.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
    const float growthSpeed = 600.f;
    const float height = 60.f;
}


ScoreBar::ScoreBar(const sf::Color& colour, float maxLength, float maxScore)
    : m_maxLength (maxLength),
    m_maxScore    (maxScore)
{
    m_shape.setFillColor(colour);
    m_shape.setSize({ 0.f, 0.f });
    m_shape.setOutlineThickness(-2.f);
    m_shape.setOutlineColor(sf::Color::Black);

    //if (m_maxLength == 0) m_maxLength = 0.00001f; //prevent div by 0
}

//public
bool ScoreBar::update(float dt)
{
    auto size = m_shape.getSize();
    size.y = height;
    if (size.x < m_maxLength)
    {
        size.x += growthSpeed * dt;
        m_shape.setSize(size);
    }

    return (m_shape.getSize().x >= m_maxLength);
}

float ScoreBar::getValue() const
{
    return (m_maxLength > 0.f) ? (m_shape.getSize().x / m_maxLength) * m_maxScore : 0.f;
}

void ScoreBar::setTexture(sf::Texture& t)
{
    t.setRepeated(true);
    m_shape.setTexture(&t);
}

//private
void ScoreBar::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_shape, states);
}