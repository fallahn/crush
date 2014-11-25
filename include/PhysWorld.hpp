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

#ifndef PHYSWORLD_H_
#define PHYSWORLD_H_

#include <Deletable.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>
#include <vector>
#include <set>
#include <cassert>

class Node;
class PhysWorld final : private sf::NonCopyable
{
public:
    enum class BodyType
    {
        Static,
        Dynamic
    };

    //pass this when creating an object for easy creation
    //of multiple objects with the same properties
    class Body;
    class BodyData final
    {
        friend class PhysWorld;
        friend class PhysWorld::Body;
    public:
        BodyData(float mass, float restitution)
            : m_mass(mass), m_inverseMass(0.f), m_restitution(restitution), m_type(BodyType::Dynamic)
        {
            assert(mass >= 0.f);
            assert(restitution >= 0.f && restitution <= 1.f);
            if (mass > 0.f)
                m_inverseMass = 1.f / mass;
        }

        void setMass(float mass)
        {
            assert(mass >= 0.f);
            m_mass = mass;
            m_inverseMass = (mass > 0.f)? 1.f / mass : 0.f;
        }

        void setRestitution(float r)
        {
            assert(r >= 0.f && r <= 1.f);
            m_restitution = r;
        }

        void setType(BodyType type)
        {
            m_type = type;
        }
    private:
        float m_mass;
        float m_inverseMass;
        float m_restitution;
        BodyType m_type;
    };

    //object in the simulation. we only need to support AABB objects
    //for this simulation
    class Body final : public Deletable
    {
        friend class PhysWorld;
        friend class Node;
    public:
        typedef std::unique_ptr<Body> Ptr;

        Body(sf::FloatRect size, const BodyData& BodyData);
        ~Body();

        const sf::Vector2f& getPosition() const;
        void applyForce(const sf::Vector2f& force);

        void setPosition(const sf::Vector2f& position);

    private:
        bool m_sleeping;
        BodyData m_bodyData;

        sf::Vector2f m_velocity;
        sf::Vector2f m_position;
        Node* m_node;
        sf::FloatRect m_aabb;

        
        void step(float dt);
        void move(const sf::Vector2f movement);
    };


    explicit PhysWorld(float gravity);
    ~PhysWorld() = default;

    //adds a new physics object to the world and returns a pointer to it
    Body* addObject(sf::FloatRect size, const BodyData& pd);
    //simulates one step by the given time
    void step(float dt);

private:
    typedef std::pair<Body*, Body*> CollisionPair;

    struct CollisionManifold
    {
        sf::Vector2f normal;
        float penetration = 0.f;
    };

    std::vector<Body::Ptr> m_bodies;
    std::set<CollisionPair> m_collisionPairs;
    std::vector<Body*> m_awakeBodies;

    sf::Vector2f m_gravity;

    bool collision(const Body::Ptr& a, const Body::Ptr& b);
    void resolveCollision(const CollisionPair& pair);
    CollisionManifold getManifold(const CollisionPair& pair);
};


#endif //PHYSWORLD_H_