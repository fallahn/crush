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

#include <Camera.hpp>
#include <Node.hpp>

Camera::Camera()
    :m_node(nullptr){}

Camera::~Camera()
{
    if (m_node)
        m_node->setCamera(nullptr);
}

void Camera::setView(sf::View view)
{
    m_view = view;
}

sf::View Camera::getView() const
{
    if (m_node)
        m_view.setCenter(m_node->getWorldPosition());
    return m_view;
}


