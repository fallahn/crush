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

//wrapper for a view objects so that it can be attached to a scene node

#ifndef CAMERA_H_
#define CAMERA_H_

#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

class Node;
class Camera final : private sf::NonCopyable
{
    friend class Node;
public:
    Camera();
    ~Camera();

    void setView(sf::View view);
    sf::View getView() const;

private:
    Node* m_node;
    mutable sf::View m_view;
};

#endif //CAMERA_H_