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

#include <Scene.hpp>

#include <cassert>

Camera Scene::defaultCamera; //TODO set the default view size

Scene::Scene()
    : m_activeCamera(nullptr)
{
    m_activeCamera = &defaultCamera;
}

//public
void Scene::addNode(Node::Ptr& node)
{
    node->setScene(this);

    if (node->getCamera() && m_activeCamera == &defaultCamera)
        m_activeCamera = node->getCamera();

    m_children.push_back(std::move(node));
}

Node::Ptr Scene::removeNode(Node& node)
{
    auto result = std::find_if(m_children.begin(), m_children.end(), [&node](const Node::Ptr& p)
    {
        return (p.get() == &node);
    });
    assert(result != m_children.end());

    Node::Ptr found = std::move(*result);
    found->setScene(nullptr);
    m_children.erase(result);
    return found;
}

void Scene::setActiveCamera(Camera* camera)
{
    m_activeCamera = camera;
}

Camera* Scene::getActiveCamera() const
{
    return m_activeCamera;
}

Node* Scene::findNode(const std::string& name, bool recursive)
{
    auto result = std::find_if(m_children.begin(), m_children.end(), [&name](const Node::Ptr& p)
    {
        return (p->getName() == name);
    });

    if (result != m_children.end()) return result->get();

    Node* np = nullptr;
    if (recursive)
    {
        for (const auto& c : m_children)
        {
            np = c->findChild(name, true);
            if (np) return np;
        }
    }
    return np;
}

void Scene::executeCommand(Command& command, float dt)
{
    for (auto& child : m_children)
        child->executeCommand(command, dt);
}

//private
void Scene::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.setView(m_activeCamera->getView());
    for (const auto& c : m_children)
        rt.draw(*c);
}