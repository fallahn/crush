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
#include <Scene.hpp>
#include <WaterDrawable.hpp>
#include <Util.hpp>

#include <cassert>
#include <iostream>

Node::Node(const std::string& name)
    : m_parent      (nullptr),
    m_name          (name),
    m_scene         (nullptr),
    m_camera        (nullptr),
    m_drawable      (nullptr),
    m_collisionBody (nullptr),
    m_category      (Category::None)
{

}

Node::~Node()
{
    if (m_camera)
        m_camera->m_node = nullptr;

    if (m_collisionBody)
    {
        m_collisionBody->deleteObject();
        m_collisionBody->m_node = nullptr;
    }
}

//public
void Node::addChild(Node::Ptr& child)
{
    child->m_parent = this;
    child->m_scene = m_scene;

    if (child->m_camera && m_scene->getActiveCamera() == &Scene::defaultCamera)
        m_scene->setActiveCamera(child->m_camera);

    m_children.push_back(std::move(child));
}

Node::Ptr Node::removeChild(Node& child)
{
    auto result = std::find_if(m_children.begin(), m_children.end(), [&child](const Node::Ptr& p)
    {
        return(p.get() == &child);
    });
    if (result != m_children.end())
    {
        Ptr found = std::move(*result);
        found->m_parent = nullptr;
        found->m_scene = nullptr;
        m_children.erase(result);
        return found;
    }

    //search recursively
    Node::Ptr np;
    for (auto& c : m_children)
    {
        np = removeChild(child);
        if (np) return np;
    }

    return std::move(np);
}

Node* Node::findChild(const std::string& name, bool recursive)
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
    cam->m_node = this;
}

void Node::setDrawable(sf::Drawable* drawable)
{
    m_drawable = drawable;
}

void Node::setCollisionBody(CollisionWorld::Body* b)
{
    if (m_collisionBody) m_collisionBody->deleteObject();

    m_collisionBody = b;
    if (m_collisionBody)
    {
        m_collisionBody->m_node = this;
        m_collisionBody->setPosition(getWorldPosition());
        m_collisionBody->addObserver(*this);
    }
}

CollisionWorld::Body* Node::getCollisionBody() const
{
    return m_collisionBody;
}

Scene* Node::getScene() const
{
    return m_scene;
}

Camera* Node::getCamera() const
{
    return m_camera;
}

const std::string& Node::getName() const
{
    return m_name;
}

void Node::setCategory(Category::Type cat)
{
    if (m_category != cat)
    {
        game::Event e;
        e.type = game::Event::Player;
        //TODO there's a better way to see which bit was set / unset but my brain is fried
        //compare states and raise an event   
        if ((m_category & Category::GrabbedOne)
            && ((cat & Category::GrabbedOne) == 0))
        {
            //player one released
            e.player.playerId = Category::PlayerOne;
            e.player.action = game::Event::PlayerEvent::Released;
        }
        else if (((m_category & Category::GrabbedOne) == 0)
            && (cat & Category::GrabbedOne))
        {
            //player one grabbed
            e.player.playerId = Category::PlayerOne;
            e.player.action = game::Event::PlayerEvent::Grabbed;
        }
        else if ((m_category & Category::GrabbedTwo)
            && ((cat & Category::GrabbedTwo) == 0))
        {
            //player two released
            e.player.playerId = Category::PlayerTwo;
            e.player.action = game::Event::PlayerEvent::Released;

        }
        else if (((m_category & Category::GrabbedTwo) == 0)
            && (cat & Category::GrabbedTwo))
        {
            //player two grabbed
            e.player.playerId = Category::PlayerTwo;
            e.player.action = game::Event::PlayerEvent::Grabbed;
        }
        
        auto pos = getWorldPosition();
        e.player.positionX = pos.x;
        e.player.positionY = pos.y;
        notify(*this, e);
        
        m_category = cat;
    }
}

sf::Uint16 Node::getCategory() const
{
    return m_category;
}

void Node::executeCommand(const Command& c, float dt)
{
    if (c.categoryMask & m_category)
        c.action(*this, dt);

    for (auto& child : m_children)
        child->executeCommand(c, dt);
}

void Node::onNotify(Subject& s, const game::Event& evt)
{
    switch (evt.type)
    {
    case game::Event::Node:
    {
        switch (evt.node.action)
        {
        case game::Event::NodeEvent::Despawn:
        {
            //let our observers know it's time to die
            game::Event e = evt;
            e.node.type = m_category;

            auto pos = m_collisionBody->getCentre();
            e.node.positionX = pos.x;
            e.node.positionY = pos.y;

            notify(*this, e);

            if (m_category == Category::PlayerOne
                || m_category == Category::PlayerTwo)
            {
                game::Event playerEvent;
                playerEvent.type = game::Event::Player;
                playerEvent.player.playerId = m_category;
                playerEvent.player.action = game::Event::PlayerEvent::Died;
                assert(m_collisionBody);

                playerEvent.player.positionX = pos.x;
                playerEvent.player.positionY = pos.y;
                notify(*this, playerEvent);
            }
        }
        break;
        case game::Event::NodeEvent::KilledNode:
        
        {
            if (evt.node.target == Category::Item)
            {
                //decide what it is player collected and announce it
                std::cout << "Player collected some shizzle!" << std::endl;
                game::Event::PlayerEvent::Item itemType 
                    = static_cast<game::Event::PlayerEvent::Item>(Util::Random::value(0, game::Event::PlayerEvent::Item::Size - 1));
                std::cout << "item type: " << itemType << std::endl;
                game::Event e;
                e.type = game::Event::Player;
                e.player.action = game::Event::PlayerEvent::GotItem;
                e.player.item = itemType;
                e.player.playerId = m_category;
                assert(m_collisionBody);
                auto pos = m_collisionBody->getCentre();
                e.player.positionX = pos.x;
                e.player.positionY = pos.y;
                notify(*this, e);
            }
            else //pass on event
            {
                sf::Int32 cat = m_category;
                cat &= ~(Category::LastTouchedOne | Category::LastTouchedTwo);
                m_category = static_cast<Category::Type>(cat);
                notify(*this, evt);
            }
        }
        break;
        case game::Event::NodeEvent::HitWater:
        {
            //notify particle system
            notify(*this, evt);
            //splash drawable
            //TODO HAAAX - this assumes because we have awater event we have a water drawble
            //attached and we call an upcast :/
            assert(m_drawable);
            auto water = static_cast<WaterDrawable*>(m_drawable);
            water->splash(evt.node.positionX - getWorldPosition().x, std::min(500.f, std::fabs(evt.node.speed)));
        }
        default: break;
        }
    }
        break;
    case game::Event::Player:
    {
        switch (evt.player.action)
        {
        default:
        {
            game::Event e = evt;
            e.player.playerId = m_category;
            assert(m_collisionBody);
            auto pos = m_collisionBody->getCentre();
            e.player.positionX = pos.x;
            e.player.positionY = pos.y;
            notify(*this, e);
        }
        break;
        case game::Event::PlayerEvent::Dropped:
            //this case come from a block body, so just pass on up
            notify(*this, evt);
            break;
        }
    }
        break;
    default: break;
    }
   
}

void Node::raiseEvent(const game::Event& evt)
{
    notify(*this, evt);
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