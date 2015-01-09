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

//slider control

#ifndef UI_SLIDER_H_
#define UI_SLIDER_H_

#include <UIControl.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace ui
{
    class Slider final : public Control
    {
    public:
        typedef std::shared_ptr<Slider> Ptr;
        enum class Direction
        {
            Horizontal,
            Vertical
        };

        Slider(const sf::Font& font, const sf::Texture& texture, float length = 250.f, float maxValue = 100.f);
        ~Slider() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event& e);

        void setMaxValue(float value);
        void setDirection(Direction direction);
        void setLength(float length);
        void setValue(float value);
        float getValue() const;

        void setText(const std::string& text);
        void setTextColour(const sf::Color& colour);
        void setFontSize(sf::Uint16 size);

    private:
        enum State
        {
            Normal = 0,
            Selected
        };

        float m_maxValue;
        float m_length;
        Direction m_direction;
        sf::FloatRect m_localBounds;

        sf::Sprite m_handleSprite;
        sf::RectangleShape m_slotShape;
        std::vector<sf::IntRect> m_subRects;
        sf::Text m_text;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const;
        void updateText();
    };
}

#endif //UI_SLIDER_H_