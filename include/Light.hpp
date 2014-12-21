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

//describes a point light used for scene lighting

#ifndef LIGHT_H_
#define LIGHT_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class Node;
class Light final
{
    friend class Node;
public:
    Light();
    Light(const sf::Vector3f& position, const sf::Vector3f& colour, float range);
    Light(const Light& copy) = default;
    ~Light() = default;

    void setPosition(const sf::Vector2f& position);
    void setPosition(const sf::Vector3f& position);
    const sf::Vector3f& getPosition() const;
    void setDepth(float zDepth);
    float getDepth() const;

    void setColour(const sf::Vector3f& colour);
    const sf::Vector3f& getColour() const;

    void setRange(float range);
    float getRange() const;
    float getRangeInverse() const;

    void setNode(Node* n);

private:
    mutable sf::Vector3f m_position;
    sf::Vector3f m_colour;
    float m_range;
    float m_rangeInverse;

    Node* m_node;
};

#endif // LIGHT_H_