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

#ifndef UI_INPUT_SELECT_H_
#define UI_INPUT_SELECT_H_

#include <UIControl.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace ui
{
    class InputSelect final : public Control
    {
    public:
        typedef std::shared_ptr<InputSelect> Ptr;

        explicit InputSelect(const sf::Font& font);
        ~InputSelect() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&) override;
        void setAlignment(Alignment) override;

    private:
        
        sf::Color m_borderColour;
        sf::Color m_selectedColour;

        sf::RectangleShape m_backShape;
        sf::Text m_text;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_INPUT_SELECT_H_