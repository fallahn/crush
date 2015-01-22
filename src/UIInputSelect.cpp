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

#include <UIInputSelect.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

InputSelect::InputSelect(const sf::Font& font)
    : m_borderColour    (),
    m_selectedColour    (m_borderColour.r / 2u, m_borderColour.g / 2u, m_borderColour.b / 2u, m_borderColour.a),
    m_text              ("", font, 18u)
{
    //TODO set up back shape
}

//public
bool InputSelect::selectable() const
{
    return true;
}

void InputSelect::select()
{
    Control::select();
    m_backShape.setOutlineColor(m_selectedColour);
}

void InputSelect::deselect()
{
    Control::deselect();
    m_backShape.setOutlineColor(m_borderColour);
}

void InputSelect::activate()
{
    Control::activate();
}

void InputSelect::deactivate()
{
    Control::deactivate();
}

void InputSelect::handleEvent(const sf::Event& e)
{

}

void InputSelect::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::BottomLeft:
        setOrigin(0.f, m_backShape.getSize().y);
        break;
    case Alignment::BottomRight:
        setOrigin(m_backShape.getSize());
        break;
    case Alignment::Centre:
        setOrigin(m_backShape.getSize() / 2.f);
        break;
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::TopRight:
        setOrigin(m_backShape.getSize().x, 0.f);
        break;
    default: break;
    }
}

//private
void InputSelect::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_backShape, states);
    rt.draw(m_text, states);
}