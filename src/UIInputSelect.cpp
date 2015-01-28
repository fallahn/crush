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
#include <InputMapping.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

namespace
{
    const float padding = 4.f;
}

InputSelect::InputSelect(const sf::Font& font, const std::string& name,  const sf::Color& borderColour)
    : m_borderColour    (borderColour),
    m_selectedColour    (m_borderColour.r / 2u, m_borderColour.g / 2u, m_borderColour.b / 2u, m_borderColour.a),
    m_text              ("", font, 18u),
    m_name              (name)
{
    m_backShape.setSize(sf::Vector2f(160.f, 28.f));
    m_backShape.setOutlineColor(borderColour);
    m_backShape.setOutlineThickness(2.f);
    m_backShape.setFillColor(sf::Color::Black);

    m_text.setPosition(padding, padding);
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
    m_text.setString("Press any key");
}

void InputSelect::deactivate()
{
    Control::deactivate();
    if (m_callback) m_callback();
}

void InputSelect::handleEvent(const sf::Event& e, const sf::Vector2f& mousePos)
{
    if (e.type == sf::Event::KeyReleased)
    {
        m_value = InputMap::toString(e.key.code);
        m_text.setString(m_value);
        deactivate();
    }
    else if (e.type == sf::Event::JoystickButtonReleased)
    {
        m_value == InputMap::toString(e.joystickButton.button);
        m_text.setString(m_value);
        deactivate();
    }
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

bool InputSelect::contains(const sf::Vector2f& mousePos) const
{
    return getTransform().transformRect(m_backShape.getGlobalBounds()).contains(mousePos);
}

const std::string& InputSelect::getName() const
{
    return m_name;
}

void InputSelect::setValue(const std::string& str)
{
    m_value = str;
    m_text.setString(str);
}

const std::string& InputSelect::getValue() const
{
    return m_value;
}

void InputSelect::setCallback(Callback cb)
{
    m_callback = std::move(cb);
}

//private
void InputSelect::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_backShape, states);
    rt.draw(m_text, states);
}