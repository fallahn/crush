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

#include <UIButton.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

Button::Button(const sf::Font& font, const sf::Texture& texture)
    : m_texture     (texture),
    m_text          ("", font, 20u),
    m_toggleButton  (false)
{
    sf::IntRect subrect({ 0, 0 }, sf::Vector2i(texture.getSize()));
    subrect.height /= 3;

    for (auto i = 0u; i < 3u; ++i)
    {
        m_subRects.push_back(subrect);
        subrect.top += subrect.height;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(m_subRects[ButtonState::Normal]);

    auto bounds = m_sprite.getLocalBounds();
    m_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

//public
bool Button::selectable() const
{
    return true;
}

void Button::select()
{
    Control::select();
    m_sprite.setTextureRect(m_subRects[ButtonState::Selected]);
}

void Button::deselect()
{
    Control::deselect();
    m_sprite.setTextureRect(m_subRects[ButtonState::Normal]);
}

void Button::activate()
{
    Control::activate();
    if (m_toggleButton)
        m_sprite.setTextureRect(m_subRects[ButtonState::Active]);

    if (m_callback) m_callback();

    if (!m_toggleButton)
        deactivate();
}

void Button::deactivate()
{
    Control::deactivate();
    if (m_toggleButton)
    {
        if (selected())
        {
            m_sprite.setTextureRect(m_subRects[ButtonState::Selected]);
        }
        else
        {
            m_sprite.setTextureRect(m_subRects[ButtonState::Normal]);
        }
    }
}

void Button::handleEvent(const sf::Event& e)
{

}

void Button::setCallback(Callback cb)
{
    m_callback = std::move(cb);
}

void Button::setText(const std::string& text)
{
    m_text.setString(text);
    Util::Position::centreOrigin(m_text);
}

void Button::setTextColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Button::setFontSize(sf::Uint16 size)
{
    m_text.setCharacterSize(size);
    Util::Position::centreOrigin(m_text);
}

void Button::setTogglable(bool b)
{
    m_toggleButton = b;
}

//private
void Button::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_sprite, states);
    rt.draw(m_text, states);
}