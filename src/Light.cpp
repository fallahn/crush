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

#include <Light.hpp>
#include <Node.hpp>

#include <cassert>

Light::Light()
    : m_colour      ({1.f, 1.f, 1.f}),
    m_range         (100.f),
    m_rangeInverse  (1.f / m_range),
    m_node          (nullptr){}

Light::Light(const sf::Vector3f& position, const sf::Vector3f& colour, float range)
    : m_position    (position),
    m_colour        (colour),
    m_range         (range),
    m_rangeInverse  (1.f / range),
    m_node          (nullptr)
{
    assert(range > 0.f);
}

//public
void Light::setPosition(const sf::Vector2f& position)
{
    if (m_node)
        m_node->setWorldPosition(position);

    m_position.x = position.x;
    m_position.y = position.y;
}

void Light::setPosition(const sf::Vector3f& position)
{
    if (m_node)
        m_node->setWorldPosition({ position.x, position.y });

    m_position = position;
}

const sf::Vector3f& Light::getPosition() const
{
    if (m_node)
    {
        auto p = m_node->getWorldPosition();
        m_position.x = p.x;
        m_position.y = p.y;
    }
    return m_position;
}

void Light::setDepth(float depth)
{
    m_position.z = std::fabs(depth);
}

float Light::getDepth() const
{
    return std::fabs(m_position.z);
}

void Light::setColour(const sf::Vector3f& colour)
{
    m_colour = colour;
}

const sf::Vector3f& Light::getColour() const
{
    return m_colour;
}

void Light::setRange(float range)
{
    assert(range > 0.f);
    m_range = range;
    m_rangeInverse = 1.f / range;
}

float Light::getRange() const
{
    return m_range;
}

float Light::getRangeInverse() const
{
    return m_rangeInverse;
}

void Light::setNode(Node* n)
{
    if (n != m_node)
    {
        m_node = n;
    } 
}

//private