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

//the root of any scene graph, responsible for managing scene lights and cameras

#ifndef SCENE_H_
#define SCENE_H_

#include <Node.hpp>

#include <set>

class Scene final : public sf::Drawable, private sf::NonCopyable, public Observer, public Subject
{
public:
    Scene();
    ~Scene() = default;

    void addNode(Node::Ptr& node);
    Node::Ptr removeNode(Node& node);

    void setActiveCamera(Camera* node);
    Camera* getActiveCamera() const;
    static Camera defaultCamera;

    Node* findNode(const std::string& name, bool recursive = true);

    void executeCommand(const Command& command, float dt);

    void onNotify(Subject& s, const game::Event& evt) override;

    //delete any nodes waiting
    void flush();

private:
    std::vector<Node::Ptr> m_children;
    Camera* m_activeCamera;

    //we want to make sure each node is only entered once
    std::set<Node*> m_deletedList;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //SCENE_H_