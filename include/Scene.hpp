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

//the root of any scene graph, responsible for managing scene lights and cameras

#ifndef SCENE_H_
#define SCENE_H_

#include <Node.hpp>
#include <Light.hpp>

#include <SFML/Graphics/Color.hpp>

#include <set>


class Scene final : public sf::Drawable, private sf::NonCopyable, public Observer, public Subject
{
public:
    enum Layer //this sets the order in which the layers are drawn
    { 
        Background,
        RearDetail,
        DynamicRear, //players / NPCs etc
        DynamicFront, //player details like hat / powerups
        Water,
        Solid,
        FrontDetail,
        LayerCount
    };

    Scene();
    ~Scene() = default;

    void addNode(Node::Ptr& node);
    void addNode(Node::Ptr&, Layer layer);
    Node::Ptr removeNode(Node& node);

    void setLayerDrawable(sf::Drawable* d, Layer layer);

    void setActiveCamera(Camera* node);
    Camera* getActiveCamera() const;
    static Camera defaultCamera;

    Light* addLight(const sf::Vector3f& colour, float range);
    void setSunlight(const Light& light);
    void addShader(sf::Shader& shader);
    void setAmbientColour(const sf::Color& colour);
    void setSunLightColour(const sf::Color& colour);

    Node* findNode(const std::string& name, bool recursive = true);

    void executeCommand(const Command& command, float dt);

    void onNotify(Subject& s, const Event& evt) override;

    void update(float dt);



private:
    std::vector<Node::Ptr> m_children;
    Camera* m_activeCamera;

    Light m_sunLight;
    sf::Vector3f m_sunDirection;
    std::vector<Light> m_lights;
    std::vector<sf::Shader*> m_shaders;
    sf::Vector3f m_ambientColour;

    //we want to make sure each node is only entered once
    std::set<Node*> m_deletedList;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    //delete any nodes waiting
    void flush();
};

#endif //SCENE_H_