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

#include <UISlider.hpp>
#include <Util.hpp>

#include <cassert>

using namespace ui;

namespace
{
    const float thickness = 5.f;
    const sf::Color borderColour(160u, 160u, 160u);
}

Slider::Slider(const sf::Font& font, const sf::Texture& texture, float length, float maxValue)
    : m_maxValue    (maxValue),
    m_length        (length),
    m_direction     (Direction::Horizontal),
    m_handleSprite  (texture),
    m_slotShape     ({ length, thickness }),
    m_text          ("", font, 20u)
{
    sf::IntRect subrect(0, 0, texture.getSize().x, texture.getSize().y / 2);
    m_subRects.push_back(subrect);
    subrect.top += subrect.height;
    m_subRects.push_back(subrect);

    m_handleSprite.setTextureRect(m_subRects[State::Normal]);

    Util::Position::centreOrigin(m_handleSprite);
    m_slotShape.setOrigin(0.f, thickness / 2.f);
    m_slotShape.setFillColor(sf::Color::Black);
    m_slotShape.setOutlineColor(borderColour);
    m_slotShape.setOutlineThickness(3.f);
}

//public
bool Slider::selectable() const
{
    return true;
}

void Slider::select()
{

}

void Slider::deselect()
{

}

void Slider::activate()
{

}

void Slider::deactivate()
{

}

void Slider::handleEvent(const sf::Event& e)
{

}

void Slider::setMaxValue(float value)
{
    assert(value > 0);
}

void Slider::setDirection(Direction direction)
{

}

void Slider::setLength(float length)
{

}

void Slider::setValue(float value)
{

}

float Slider::getValue() const
{
    return 0.f;
}

void Slider::setText(const std::string& text)
{

}

void Slider::setTextColour(const sf::Color& colour)
{

}

void Slider::setFontSize(sf::Uint16 size)
{

}

//private
void Slider::draw(sf::RenderTarget& rt, sf::RenderStates states)const
{

}

void Slider::updateText()
{

}