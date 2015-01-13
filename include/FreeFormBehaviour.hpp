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

//behaviour for freeform objects, whimsical items at the beck and call of all others....

#ifndef FREEFORM_BEHAVIOUR_H_
#define FREEFORM_BEHAVIOUR_H_

#include <BodyBehaviour.hpp>

class FreeFormBehaviourAir final : public BodyBehaviour
{
public:
    explicit FreeFormBehaviourAir(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
};

class FreeFormBehaviourGround final : public BodyBehaviour
{
public:
    explicit FreeFormBehaviourGround(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
private:

};

class FreeFormBehaviourCarry final : public BodyBehaviour
{
public:
    explicit FreeFormBehaviourCarry(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
private:

};

class FreeFormBehaviourWater final : public BodyBehaviour
{
public:
    explicit FreeFormBehaviourWater(CollisionWorld::Body* b) : BodyBehaviour(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body* other) override;
private:

};

#endif //FREEFORM_BEHAVIOUR