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

//animated UI icon which bounces / pulses

#ifndef ANIMATED_ICON_H_
#define ANIMATED_ICON_H_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

class AnimatedIcon final : public sf::Drawable, public sf::Transformable
{
public:
    explicit AnimatedIcon(const sf::Texture& texture);
    AnimatedIcon(const AnimatedIcon& copy) = default;
    ~AnimatedIcon() = default;

    void update(float dt);
    void setColour(const sf::Color& c);

private:
    sf::Uint8 m_tableIndex;
    sf::Sprite m_sprite;
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //ANIMATED_ICON_H_