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

//represents a node used to build a scene graph. Nodes are the basis for attaching components
//such as controllers, physics object and drawables

#ifndef NODE_H_
#define NODE_H_

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <Camera.hpp>
#include <PhysWorld.hpp>

#include <vector>
#include <memory>


class Scene;
class Node final : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<Node> Ptr;

    explicit Node(const std::string& name = "");
    ~Node();

    void addChild(Ptr& child);
    Ptr removeChild(Node& child);
    Node* findChild(const std::string& name, bool recursive = true);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

    void setWorldPosition(sf::Vector2f position);

    void setScene(Scene* scene); //this should only be accessable by Scene
    void setCamera(Camera* camera);
    void setDrawable(sf::Drawable* drawable);
    void setPhysBody(PhysWorld::Body* b);

    Scene* getScene() const;
    Camera* getCamera() const;

    const std::string& getName() const;

private:
    std::vector<Ptr> m_children;
    Node* m_parent;

    std::string m_name;

    Scene* m_scene;
    Camera* m_camera;
    sf::Drawable* m_drawable;
    PhysWorld::Body* m_physBody;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    void drawSelf(sf::RenderTarget& rt, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& rt, sf::RenderStates states) const;
};

#endif //NODE_H_