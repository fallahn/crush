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

//handles game collision detection

#ifndef COLLISION_WORLD_H_
#define COLLISION_WORLD_H_

#include <Deletable.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <set>
#include <vector>

class Node;
class BodyState;
class CollisionWorld final : sf::NonCopyable
{
public:
    class Body final : public Deletable
    {
        friend class Node;
        friend class CollisionWorld;
    public:
        typedef std::unique_ptr<Body> Ptr;
        typedef std::unique_ptr<BodyState> StatePtr;
        enum class Type
        {
            Block,
            Solid, //ie the ground
            Player,
            Npc
        };

        Body(Type type, const sf::Vector2f& size);
        ~Body();
       
        void applyForce(const sf::Vector2f& force);
        void setPosition(const sf::Vector2f& position);

        //TODO we don't really want these being completely public
        const sf::Vector2f& getVelocity() const;
        void setVelocity(const sf::Vector2f& vel);
        void move(const sf::Vector2f& distance);
        template <typename T>
        void setState(std::unique_ptr<T>& ptr)
        {
            m_nextState = std::move(ptr);
        }
        sf::Uint16 getFootSenseCount() const;

    private:
        Type m_type;
        StatePtr m_state;
        StatePtr m_nextState;
       
        sf::Vector2f m_velocity;
        sf::Vector2f m_position;
       
        Node* m_node;
        sf::FloatRect m_aabb;
        sf::FloatRect m_lastPenetration;

        sf::FloatRect m_footSensor;
        sf::Uint16 m_footSenseCount;

        void step(float dt);
    };
    
    explicit CollisionWorld(float gravity);
    ~CollisionWorld() = default;

    Body* addBody(Body::Type type, const sf::Vector2f& size);

    void step(float dt);

private:
    typedef std::pair<Body*, Body*> CollisionPair;

    std::vector<Body::Ptr> m_bodies;
    std::set<CollisionPair> m_collisions;

    sf::Vector2f m_gravity;
    //contains the normal in the first two components and penetration in z
    sf::Vector3f getManifold(const CollisionPair& cp);
};


#endif //COLLISION_WORLD_H_