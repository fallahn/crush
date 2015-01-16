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

#ifndef SCORE_BAR_H_
#define SCORE_BAR_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/NonCopyable.hpp>

class ScoreBar final : public sf::Drawable, public sf::Transformable//, private sf::NonCopyable
{
public:
    ScoreBar(const sf::Color& colour, float maxSize, float maxScore);
    ScoreBar(const ScoreBar& copy) = default;
    ~ScoreBar() = default;

    bool update(float dt); //returns true if animation complete
    float getValue() const;
    void setTexture(sf::Texture& t);

private:

    sf::RectangleShape m_shape;
    float m_maxLength;
    float m_maxScore;
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //SCORE_BAR_H_