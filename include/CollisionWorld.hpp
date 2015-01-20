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

//handles game collision detection

#ifndef COLLISION_WORLD_H_
#define COLLISION_WORLD_H_

#include <Deletable.hpp> //TODO the observer makes this redundant
#include <Observer.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <set>
#include <vector>
#include <map>

class Node;
class BodyBehaviour;
class CollisionWorld final : sf::NonCopyable
{
public:
    class Body final : public Deletable, public Subject, private sf::NonCopyable
    {
        friend class Node;
        friend class CollisionWorld;
        friend class BodyBehaviour;
    public:
        typedef std::unique_ptr<Body> Ptr;
        typedef std::unique_ptr<BodyBehaviour> BehaviourPtr;
        enum Type
        {
            Block    = (1 << 0),
            Solid    = (1 << 1), //ie the ground
            Player   = (1 << 2),
            Npc      = (1 << 3),
            Item     = (1 << 4),
            Water    = (1 << 5),
            FreeForm = (1 << 6),
            Anchor   = (1 << 7)
        };

        Body(Type type, const sf::Vector2f& size);
        ~Body();
       
        void applyForce(const sf::Vector2f& force);
        void setPosition(const sf::Vector2f& position);
        void setGravityAmount(float amount);
        void setFriction(float friction);
        float getFriction() const;
        void setStrength(float strength);
        Type getType() const;
        Category::Type getParentCategory() const;
        sf::Vector2f getSize() const;

        sf::Vector2f getCentre() const; //centre of body in world coordinates
        bool contains(const sf::Vector2f& point) const;
        
        void addChild(Body* b, const sf::Vector2f& relPosition);
        void removeChild(Body* b);
        void flipChildren();

        float getSpeed() const;
        const sf::Vector2f& getVelocity() const;

    private:
        Type m_type;
        BehaviourPtr m_behaviour;
        BehaviourPtr m_nextBehaviour;
       
        sf::Vector2f m_velocity;
        sf::Vector2f m_position;
        sf::Vector2f m_centre;
       
        Node* m_node;
        sf::FloatRect m_aabb;

        sf::FloatRect m_footSensor;
        sf::Uint16 m_footSenseCount;
        sf::Uint32 m_footSenseMask;

        float m_gravityAmount;
        float m_friction;
        float m_health;
        float m_strength;

        std::map<Body*, sf::Vector2f> m_children;
        Body* m_parent;

        bool m_dead;
        bool m_invincible;
        float m_invincibilityCount;

        void step(float dt);
        void applyGravity(const sf::Vector2f& gravity);
        void move(const sf::Vector2f& distance);
        void destroy();
        bool hasChild(Type type);
    };
    
    class Constraint final : public Deletable
    {
        friend class CollisionWorld;
    public:
        Constraint(Body* a, Body* b, float length);
        ~Constraint() = default;

        void update(float dt);
    private:
        Body* m_bodyA;
        Body* m_bodyB;
        float m_length;
    };

    explicit CollisionWorld(float gravity);
    ~CollisionWorld() = default;

    Body* addBody(Body::Type type, const sf::Vector2f& size);
    void addConstraint(Body* a, Body* b, float length);

    void step(float dt);

private:
    typedef std::pair<Body*, Body*> CollisionPair;

    std::vector<Body::Ptr> m_bodies;
    std::set<CollisionPair> m_collisions;

    std::vector<Constraint> m_constraints;

    sf::Vector2f m_gravity;
    //contains the normal in the first two components and penetration in z
    sf::Vector3f getManifold(const CollisionPair& cp);
};


#endif //COLLISION_WORLD_H_