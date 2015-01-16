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

    const float maxScale = 25.f;
}

AnimatedIcon::AnimatedIcon(const sf::Texture& texture)
    : m_fadeDuration    (0.5f),
    m_fadeTime          (m_fadeDuration),
    m_transparency      (1.f),
    m_tableIndex        (0u),
    m_sprite            (texture)
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
    m_transparency = static_cast<float>(c.a) / 255.f;
    m_fadeTime = m_fadeDuration * m_transparency;
}

void AnimatedIcon::setFadeDuration(float value)
{
    assert(value > 0);
    m_fadeDuration = value;
    m_fadeTime = m_fadeDuration * value;
}

void AnimatedIcon::setTransparency(float v)
{
    assert(v >= 0.f && v <= 1.f);
    m_transparency = v;
    m_fadeTime = m_fadeDuration * v;

    auto c = m_sprite.getColor();
    c.a = static_cast<sf::Uint8>(v * 255.f);
    m_sprite.setColor(c);
}

bool AnimatedIcon::fadeIn(float dt)
{
    m_fadeTime += dt;
    m_transparency = std::min(m_fadeTime / m_fadeDuration, 1.f);
    
    auto c = m_sprite.getColor();
    c.a = static_cast<sf::Uint8>(m_transparency * 255.f);
    m_sprite.setColor(c);

    float scale = (1.f - m_transparency) * maxScale + 1.f;
    setScale(scale, scale);

    return !(m_transparency < 1.f);
}

bool AnimatedIcon::fadeOut(float dt)
{
    m_fadeTime -= dt;
    m_transparency = std::max(m_fadeTime / m_fadeDuration, 0.f);

    auto c = m_sprite.getColor();
    c.a = static_cast<sf::Uint8>(m_transparency * 255.f);
    m_sprite.setColor(c);

    float scale = (1.f - m_transparency) * maxScale + 1.f;
    setScale(scale, scale);

    return !(m_transparency > 0.f);
}

//private
void AnimatedIcon::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_sprite, states);
}