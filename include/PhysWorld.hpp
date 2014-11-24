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
    //pass this when creating an object for easy creation
    //of multiple objects with the same properties
    struct PhysData
    {
        float mass;
        float inverseMass;
        float restitution;

        PhysData(float mass, float restitution)
            : mass(mass), inverseMass(0.f), restitution(restitution)
        {
            assert(mass >= 0.f);
            assert(restitution >= 0.f && restitution <= 1.f);
            if (mass > 0.f)
                inverseMass = 1.f / mass;
        }
    };

    //object in the simulation. we only need to support AABB objects
    //for this simulation
    class PhysObject final : public Deletable
    {
        friend class PhysWorld;
        friend class Node;
    public:
        typedef std::unique_ptr<PhysObject> Ptr;

        PhysObject(sf::FloatRect size, const PhysData& physData);
        ~PhysObject();

        const sf::Vector2f& getPosition() const;
        void addForce(const sf::Vector2f& force);

    private:
        bool m_sleeping;
        PhysData m_physData;

        sf::Vector2f m_velocity;
        sf::Vector2f m_position;
        Node* m_node;
        sf::FloatRect m_aabb;

        void setPosition(const sf::Vector2f& position);
        void step(float dt);
    };


    explicit PhysWorld(float gravity);
    ~PhysWorld() = default;

    //adds a new physics object to the world and returns a pointer to it
    PhysObject* addObject(sf::FloatRect size, const PhysData& pd);
    //simulates one step by the given time
    void step(float dt);

private:
    typedef std::pair<PhysObject*, PhysObject*> CollisionPair;

    std::vector<PhysObject::Ptr> m_objects;
    std::set<CollisionPair> m_collisionPairs;
    std::vector<PhysObject*> m_awakeObjects;

    sf::Vector2f m_gravity;

    bool collision(const PhysObject::Ptr& a, const PhysObject::Ptr& b);
    void resolveCollision(const CollisionPair& pair);
    sf::Vector2f getCollisionNormal(const CollisionPair& pair);
};


#endif //PHYSWORLD_H_