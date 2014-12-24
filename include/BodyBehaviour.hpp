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

//sets the state of a collision body which defines its behaviour

#ifndef BODY_BEHAVIOUR_H_
#define BODY_BEHAVIOUR_H_

#include <CollisionWorld.hpp>
#include <Observer.hpp>

class BodyBehaviour : private sf::NonCopyable
{
public:
    explicit BodyBehaviour(CollisionWorld::Body* b);
    virtual ~BodyBehaviour() = default;

    virtual void update(float dt) = 0;
    virtual void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) = 0;
    virtual sf::Vector2f vetForce(const sf::Vector2f& vel){ return vel; }

protected:
    CollisionWorld::Body* getBody() const;
    //by adding accessors to base class we can allow
    //states access to body's privates without a tower of friendship
    const sf::Vector2f& getVelocity() const;
    void setVelocity(const sf::Vector2f& vel);
    void move(const sf::Vector2f& distance);

    template <typename T>
    void setBehaviour()
    {
        m_body->m_nextBehaviour = std::make_unique<T>(m_body);
    }

    sf::Uint16 getFootSenseCount() const;
    sf::Uint32 getFootSenseMask() const;
    float getFriction() const;
    Category::Type getParentCategory() const;
    void setParentCategory(Category::Type type);

    //notifys the body that it should die
    void kill();
    void damage(float amount, CollisionWorld::Body* damager);
    void raiseEvent(const Event& evt, CollisionWorld::Body* target = nullptr);

private:
    CollisionWorld::Body* m_body;
};

#endif //COLLISION_STATE_H_