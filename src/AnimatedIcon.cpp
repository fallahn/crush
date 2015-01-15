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

#include <AnimatedIcon.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>

namespace
{
    const std::vector<float> waveTable = { 0.f, 0.19f, 0.38f, 0.56f, 0.71f, 0.84f, 0.93f, 0.98f, 0.99f, 0.97f, 0.9f, 0.8f, 0.67f, 0.51f, 0.33f, 0.14f,
        0.f, -0.38f, -0.71f, -0.93f, -0.99f, -0.9f, -0.67f, -0.33f };
}

AnimatedIcon::AnimatedIcon(const sf::Texture& texture)
    : m_tableIndex  (0u),
    m_sprite        (texture)
{
    Util::Position::centreOrigin(m_sprite);
}

//public
void AnimatedIcon::update(float dt)
{
    m_tableIndex = (m_tableIndex + 1) % waveTable.size();
    float scale = waveTable[m_tableIndex] * 0.2f + 1.f;
    m_sprite.setScale(scale, scale);
}

void AnimatedIcon::setColour(const sf::Color& c)
{
    m_sprite.setColor(c);
}

//private
void AnimatedIcon::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_sprite, states);
}