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

#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <State.hpp>
#include <Scene.hpp>
#include <CollisionWorld.hpp>
#include <Player.hpp>
#include <NpcController.hpp>
#include <ScoreBoard.hpp>
#include <ParticleController.hpp>
#include <MapController.hpp>

class GameState final : public State
{
public:
    GameState(StateStack& stack, Context context);
    ~GameState() = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;

private:

    Scene m_scene;
    CommandStack m_commandStack;
    CollisionWorld m_collisionWorld;
    std::vector<Player> m_players;
    NpcController m_npcController;
    ScoreBoard m_scoreBoard;
    ParticleController m_particleController;
    MapController m_mapController;

    void addBlock(const sf::Vector2f& position, const sf::Vector2f& size);
    void addPlayer(const sf::Vector2f& position, Player& player);
    void addNpc(const sf::Vector2f& position);
    void addMapBody(Category::Type type, const sf::Vector2f& position, const sf::Vector2f& size = sf::Vector2f());
};

#endif //GAME_STATE_H_