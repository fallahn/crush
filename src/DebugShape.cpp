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

#include <DebugShape.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const float padding = 10.f;
}

DebugShape::DebugShape()
{
    m_mainShape.setFillColor(sf::Color::Transparent);
    m_mainShape.setOutlineThickness(-3.f);
    m_mainShape.setSize({ 10.f, 10.f });

    m_footShape = m_mainShape;
    m_footShape.setPosition(0.f, m_mainShape.getSize().y);
    m_footShape.setSize({ m_mainShape.getSize().x, padding });
    m_footShape.setOutlineThickness(-1.f);
}

//public
void DebugShape::setColour(const sf::Color& colour)
{
    m_mainShape.setOutlineColor(colour);
}

void DebugShape::setSize(const sf::Vector2f& size)
{
    m_mainShape.setSize(size);
    m_footShape.setSize({ size.x, padding });
    m_footShape.setPosition(0.f, size.y);
}

const sf::Vector2f& DebugShape::getSize() const
{
    return m_mainShape.getSize();
}

//private
void DebugShape::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_footShape, states);
    rt.draw(m_mainShape, states);
}