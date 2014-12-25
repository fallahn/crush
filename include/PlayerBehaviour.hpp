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

//defines behaviour of player bodies

#ifndef PLAYER_BEHAVIOUR_H_
#define PLAYER_BEHAVIOUR_H_

#include <BodyBehaviour.hpp>

class PlayerBehaviourAir final : public BodyBehaviour
{
public:
    explicit PlayerBehaviourAir(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
    sf::Vector2f vetForce(const sf::Vector2f& force) override;
private:
    sf::Vector2f m_lastVelocity;
};

class PlayerBehaviourGround final : public BodyBehaviour
{
public:
    explicit PlayerBehaviourGround(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
private:
    sf::Vector2f m_lastVelocity;
};

class PlayerBehaviourWater final : public BodyBehaviour
{
public:
    explicit PlayerBehaviourWater(CollisionWorld::Body* b);
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
private:
    float m_sinkTime;
    float m_currentTime;
    bool m_splashed;
};

#endif // PLAYER_BEHAVIOUR_H_