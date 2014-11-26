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

#ifndef BODY_STATE_H_
#define BODY_STATE_H_

#include <CollisionWorld.hpp>

class BodyState
{
public:
    explicit BodyState(CollisionWorld::Body* b);
    virtual ~BodyState() = default;

    virtual void update(float dt) = 0;
    virtual void resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType) = 0;

protected:
    CollisionWorld::Body* getBody() const;
private:
    CollisionWorld::Body* m_body;
};

class BlockStateAir final : public BodyState
{
public:
    explicit BlockStateAir(CollisionWorld::Body* b) : BodyState(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType) override;
};

class BlockStateGround final : public BodyState
{
public:
    explicit BlockStateGround(CollisionWorld::Body* b) : BodyState(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType) override;
};

class SolidState final : public BodyState
{
public:
    explicit SolidState(CollisionWorld::Body* b) : BodyState(b){};
    void update(float dt) override;
    void resolve(const sf::Vector3f& manifold, CollisionWorld::Body::Type otherType) override;
};

#endif //COLLISION_STATE_H_