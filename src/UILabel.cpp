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

#include <UILabel.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

Label::Label(const std::string& text, const sf::Font& font)
    : m_text(text, font, 20u)
{

}

//public
bool Label::selectable() const
{
    return false;
}

void Label::handleEvent(const sf::Event& e){}

void Label::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::BottomLeft:
        setOrigin(0.f, m_text.getLocalBounds().height);
        break;
    case Alignment::Centre:
    {
        auto size = m_text.getLocalBounds();
        setOrigin(sf::Vector2f(size.width, size.height) / 2.f);
    }
        break;
    case Alignment::TopRight:
        setOrigin(m_text.getLocalBounds().width, 0.f);
        break;
    case Alignment::BottomRight:
    {
        auto size = m_text.getLocalBounds();
        setOrigin(sf::Vector2f(size.width, size.height));
    }
        break;
    default:break;
    }
}

void Label::setText(const std::string& text)
{
    m_text.setString(text);
}

void Label::setTextColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Label::setFont(const sf::Font& font)
{
    m_text.setFont(font);
}

void Label::setFontSize(sf::Uint16 size)
{
    m_text.setCharacterSize(size);
}

//private
void Label::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_text, states);
}