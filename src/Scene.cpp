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
#include <SFML/Graphics/Shader.hpp>

#include <Scene.hpp>

#include <cassert>
#include <array>

namespace
{
    const sf::Uint8 maxLights = 3;
    const sf::Vector3f sunTarget(960.f, 540.f, 0.f);
}

Camera Scene::defaultCamera;

Scene::Scene()
    : m_activeCamera    (nullptr),
    m_ambientColour     ({0.2f, 0.2f, 0.2f}),
    m_sunLight          ({ 980.f, 500.f, 30.f }, {1.f, 0.99f, 0.9f}, 1.f)
{
    m_activeCamera = &defaultCamera;

    for (auto i = 0; i < Layer::LayerCount; ++i)
    {
        auto n = std::make_unique<Node>();
        addNode(n);
    }

    m_lights.reserve(maxLights);
    m_sunDirection = m_sunLight.getPosition() - sunTarget;
}

//public
void Scene::addNode(Node::Ptr& node)
{
    node->setScene(this);
    node->addObserver(*this);

    if (node->getCamera() && m_activeCamera == &defaultCamera)
        m_activeCamera = node->getCamera();

    //announce our arrival in the scene
    game::Event e;
    e.type = game::Event::Node;
    e.node.action = game::Event::NodeEvent::Spawn;
    e.node.type = static_cast<Category::Type>(node->getCategory());
    e.node.target = Category::None;
    e.node.owner = Category::None;
    auto pos = (node->getCollisionBody()) ? node->getCollisionBody()->getCentre() : node->getWorldPosition();
    e.node.positionX = pos.x;
    e.node.positionY = pos.y;
    notify(*this, e);

    m_children.push_back(std::move(node));
}

void Scene::addNode(Node::Ptr& node, Layer layer)
{
    node->addObserver(*this);

    //announce our arrival in the scene
    game::Event e;
    e.type = game::Event::Node;
    e.node.action = game::Event::NodeEvent::Spawn;
    e.node.type = static_cast<Category::Type>(node->getCategory());
    e.node.target = Category::None;
    e.node.owner = Category::None;
    auto pos = (node->getCollisionBody()) ? node->getCollisionBody()->getCentre() : node->getWorldPosition();
    e.node.positionX = pos.x;
    e.node.positionY = pos.y;
    notify(*this, e);

    m_children[layer]->addChild(node);
}

Node::Ptr Scene::removeNode(Node& node)
{
    auto result = std::find_if(m_children.begin(), m_children.end(), [&node](const Node::Ptr& p)
    {
        return (p.get() == &node);
    });

    if(result != m_children.end())
    {
        Node::Ptr found = std::move(*result);
        found->setScene(nullptr);
        m_children.erase(result);
        return found;
    }
    //else search recursively
    Node::Ptr np;
    for (auto& c : m_children)
    {
        np = c->removeChild(node);
        if (np) return np;
    }
    return std::move(np);
}

void Scene::setLayerDrawable(sf::Drawable* d, Layer layer)
{
    m_children[layer]->setDrawable(d);
}

void Scene::setActiveCamera(Camera* camera)
{
    m_activeCamera = camera;
}

Camera* Scene::getActiveCamera() const
{
    return m_activeCamera;
}

Light* Scene::addLight(const sf::Vector3f& colour, float range)
{
    assert(m_lights.size() <= maxLights);
    m_lights.emplace_back(sf::Vector3f(0.f, 0.f, 20.f), colour, range);
    return &m_lights.back();
}

void Scene::setSunlight(const Light& light)
{
    m_sunLight = light;
    m_sunDirection = m_sunLight.getPosition() - sunTarget;
}

void Scene::addShader(sf::Shader& shader)
{
    m_shaders.push_back(&shader);
}

void Scene::setAmbientColour(const sf::Color& colour)
{
    m_ambientColour.x = static_cast<float>(colour.r) / 255.f;
    m_ambientColour.y = static_cast<float>(colour.g) / 255.f;
    m_ambientColour.z = static_cast<float>(colour.b) / 255.f;
}

void Scene::setSunLightColour(const sf::Color& colour)
{
    m_sunLight.setColour({ static_cast<float>(colour.r) / 255.f,
                            static_cast<float>(colour.g) / 255.f,
                            static_cast<float>(colour.b) / 255.f });
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

void Scene::executeCommand(const Command& command, float dt)
{
    for (auto& child : m_children)
        child->executeCommand(command, dt);
}

void Scene::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Node:
        if (evt.node.action == game::Event::NodeEvent::Despawn)
            m_deletedList.insert(dynamic_cast<Node*>(&s)); //HAH! ok...
        break;
    default: break;
    }
}

void Scene::update(float dt)
{
    //this assumes all shaders require light data updating
    //ready for use by any nodes

    //sfml doesn't support array uniforms so this is fudged by using the
    //underlying array of the transform class
    std::array<sf::Vector3f, maxLights> positions;
    std::array<sf::Vector3f, maxLights> colours;
    std::array<float, maxLights> ranges = { 1.f, 1.f, 1.f };

    for (auto i = 0u; i < m_lights.size(); ++i)
    {
        positions[i] = m_lights[i].getPosition();
        colours[i] = m_lights[i].getColour();
        ranges[i] = m_lights[i].getRangeInverse();
    }

    sf::Transform lightPositions(positions[0].x, positions[0].y, positions[0].z,
                                positions[1].x, positions[1].y, positions[1].z, 
                                positions[2].x, positions[2].y, positions[2].z);

    sf::Transform lightColours(colours[0].x, colours[0].y, colours[0].z,
                                colours[1].x, colours[1].y, colours[1].z,
                                colours[2].x, colours[2].y, colours[2].z);

    for (auto& s : m_shaders)
    {
        s->setParameter("u_directionalLightDirection", m_sunDirection);
        s->setParameter("u_directionalLightColour", m_sunLight.getColour());
        s->setParameter("u_ambientColour", m_ambientColour);

        s->setParameter("u_pointLightColours", lightColours);
        s->setParameter("u_inverseRanges", sf::Vector3f(ranges[0], ranges[1], ranges[2]));
        s->setParameter("u_pointLightPositions", lightPositions);
    }

    flush();
}

//private
void Scene::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.setView(m_activeCamera->getView());
    for (const auto& c : m_children)
        rt.draw(*c);
}

void Scene::flush()
{
    if (m_deletedList.size())
    {
        for (auto n : m_deletedList)
            removeNode(*n);

        m_deletedList.clear();
    }
}
