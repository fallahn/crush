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

#include <UITextBox.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

using namespace ui;

namespace
{
    const float borderThickness = 2.f;
    const sf::Vector2f defaultSize(320.f, 70.f);
    const float flashTime = 0.8f;
    const float padding = 20.f;
}

TextBox::TextBox(const sf::Font& font, const sf::Color& backColour, const sf::Color& borderColour)
    : m_text("", font, 30u),
    m_cursorShape({}),
    m_showCursor(false),
    m_lastKey(sf::Keyboard::Unknown)
{
    m_backShape.setFillColor(backColour);
    m_backShape.setOutlineColor(borderColour);
    m_backShape.setOutlineThickness(borderThickness);
    m_backShape.setSize(defaultSize);
    //TODO set origin?

    m_text.setColor(borderColour);
    m_text.setPosition(padding, 0.f);
}

//public
bool TextBox::selectable() const
{
    return true;
}

void TextBox::select()
{
    Control::select();
    //TODO change border colour to selected
}

void TextBox::deselect()
{
    Control::deselect();
    //TODO return to normal border colour
}

void TextBox::activate()
{
    Control::activate();
    m_showCursor = true;
}

void TextBox::deactivate()
{
    Control::deactivate();
    m_showCursor = false;
}

void TextBox::handleEvent(const sf::Event& e)
{
    if (m_showCursor) //TODO why aren't we just checking if we're active?
    {
        if (e.type == sf::Event::KeyReleased)
        {
            if (e.key.code == sf::Keyboard::BackSpace)
            {
                if (m_string.size())
                {
                    m_string.pop_back();
                }
            }
        }
    }

    else if (e.type == sf::Event::TextEntered)
    {
        if (e.text.unicode > 31
            && e.text.unicode < 127)
        {
            m_string += static_cast<char>(e.text.unicode);
        }
    }
}

void TextBox::update(float dt)
{
    sf::FloatRect bounds = m_text.getGlobalBounds();
    m_cursorShape.setPosition(bounds.left + bounds.width, -(m_cursorShape.getSize().y / 2.f));
    m_text.setString(m_string);
}

const std::string& TextBox::getText() const
{
    return m_string;
}

void TextBox::setTexture(const sf::Texture& t)
{
    m_backShape.setTexture(&t);
}

void TextBox::showBorder(bool show)
{
    (show) ? 
        m_backShape.setOutlineThickness(borderThickness) : 
        m_backShape.setOutlineThickness(0.f);
}

void TextBox::setSize(const sf::Vector2f& size)
{
    m_backShape.setSize(size);
    m_cursorShape.setScale(1.f, size.y / defaultSize.y);

    sf::Uint8 charSize = static_cast<sf::Uint8>(size.y * 0.75f);
    m_text.setCharacterSize(charSize);
    //TODO set text padding?
}

void TextBox::setText(const std::string& text)
{
    m_string = text;
}

//private
void TextBox::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_backShape, states);
    rt.draw(m_text, states);
    if (m_showCursor) rt.draw(m_cursorShape, states);
}