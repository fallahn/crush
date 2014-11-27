/*********************************************************************
Matt Marchant 2014
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

//special drawable to make visual debugging easier

#ifndef DEBUG_SHAPE_H_
#define DEBUG_SHAPE_H_

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class DebugShape final : public sf::Transformable, public sf::Drawable
{
public:
    DebugShape();
    ~DebugShape() = default;

    void setColour(const sf::Color& colour);
    void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;

private:
    sf::RectangleShape m_mainShape;
    sf::RectangleShape m_footShape;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //DEBUG_SHAPE_H_