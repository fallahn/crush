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

//defines behaviour of NPC bodies

#ifndef NPC_BEHAVIOUR_H_
#define NPC_BEHAVIOUR_H_

#include <BodyBehaviour.hpp>

class NpcBehaviourAir final : public BodyBehaviour
{
public:
    explicit NpcBehaviourAir(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
};

class NpcBehaviourGround final : public BodyBehaviour
{
public:
    explicit NpcBehaviourGround(CollisionWorld::Body* b);
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;

private:
    float m_changeDelay;
    float m_accumulatedTime;
};

class NpcBehaviourWalk final : public BodyBehaviour
{
public:
    explicit NpcBehaviourWalk(CollisionWorld::Body* b);
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;

private:
    float m_changeDelay;
    float m_accumulatedTime;
    float m_moveForce;
};

#endif //NPC_BEHAVIOUR_H_