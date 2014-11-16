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
#include <SFML/Graphics/RenderTarget.hpp>

#include <Node.hpp>

#include <cassert>

Node::Node()
    : m_parent(nullptr),
    m_scene(nullptr),
    m_camera(nullptr),
    m_drawable(nullptr)
{

}

Node::~Node()
{
    if (m_camera)
        m_camera->m_node = nullptr;
}

//public
void Node::addChild(Node::Ptr& child)
{
    child->m_parent = this;
    child->m_scene = m_scene;
    m_children.push_back(std::move(child));
}

Node::Ptr Node::removeChild(Node& child)
{
    auto result = std::find_if(m_children.begin(), m_children.end(), [&child](const Node::Ptr& p)
    {
        return(p.get() == &child);
    });
    assert(result != m_children.end());

    Ptr found = std::move(*result);
    found->m_parent = nullptr;
    found->m_scene = nullptr;
    m_children.erase(result);
    return found;

}

sf::Vector2f Node::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform Node::getWorldTransform() const
{
    auto t = sf::Transform::Identity;
    for (const auto* node = this; node != nullptr; node = node->m_parent)
        t = node->getTransform() * t;

    return t;
}


void Node::setWorldPosition(sf::Vector2f position)
{
    if (m_parent)
        position -= m_parent->getWorldPosition();

    setPosition(position);
}

void Node::setScene(Scene* scene)
{
    m_scene = scene;
}

void Node::setCamera(Camera* cam)
{
    m_camera = cam;
}

void Node::setDrawable(sf::Drawable* drawable)
{
    m_drawable = drawable;
}

Scene* Node::getScene() const
{
    return m_scene;
}

Camera* Node::getCamera() const
{
    return m_camera;
}

//private
void Node::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawSelf(rt, states);
    drawChildren(rt, states);
}

void Node::drawSelf(sf::RenderTarget& rt, sf::RenderStates states) const
{
    if (m_drawable)
        rt.draw(*m_drawable, states);
}

void Node::drawChildren(sf::RenderTarget& rt, sf::RenderStates states) const
{
    for (const auto& c : m_children)
        c->draw(rt, states);
}