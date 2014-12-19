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

#include <GameState.hpp>
#include <Game.hpp>
#include <DebugShape.hpp>
#include <BodyBehaviour.hpp>
#include <Util.hpp>
#include <Particles.hpp>
#include <Map.hpp>
#include <WaterDrawable.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>
#include <list>

namespace
{
    Camera sceneCam;

    //temporary just to hold solid shapes until sprite / texturing is implemented
    std::vector<sf::RectangleShape> shapes; //so we can reuse shapes
    sf::RectangleShape solidShape;

    DebugShape blockShape;
    DebugShape playerShape;
    DebugShape npcShape;

    std::list<WaterDrawable> waterDrawables;

    const sf::Uint8 maxPlayers = 2u;

    sf::Vector2f blockSize(60.f, 40.f);
 
    TextureResource textureResource;
}

GameState::GameState(StateStack& stack, Context context)
    : State             (stack, context),
    m_collisionWorld    (70.f),
    m_npcController     (m_commandStack),
    m_scoreBoard        (stack, context),
    m_particleController(textureResource),
    m_mapController     (m_commandStack, textureResource)
{
    //build world
    getContext().renderWindow.setTitle("Game Screen");
    
    Scene::defaultCamera.setView(getContext().defaultView);

    //TODO remove shapes for sprites managed by controllers
    shapes.reserve(50); //TODO temp stuffs
    solidShape.setFillColor(sf::Color::Transparent);
    solidShape.setOutlineColor(sf::Color(205u, 92u, 92u));
    solidShape.setOutlineThickness(-3.f);

    blockShape.setSize(blockSize);
    blockShape.setColour(sf::Color::Red);

    playerShape.setSize(blockSize);
    playerShape.setColour(sf::Color::Blue);

    npcShape.setSize(blockSize);
    npcShape.setColour(sf::Color::Green);

    //set up controllers
    m_players.reserve(2);
    m_players.emplace_back(m_commandStack, Category::PlayerOne);
    m_players.emplace_back(m_commandStack, Category::PlayerTwo);

    std::function<void(const sf::Vector2f&, Player&)> playerSpawnFunc = std::bind(&GameState::addPlayer, this, std::placeholders::_1, std::placeholders::_2);
    m_players[0].setSpawnFunction(playerSpawnFunc);
    m_players[1].setSpawnFunction(playerSpawnFunc);

    std::function<void(const sf::Vector2f&)> npcSpawnFunc = std::bind(&GameState::addNpc, this, std::placeholders::_1);
    m_npcController.setSpawnFunction(npcSpawnFunc);

    std::function<void(Category::Type, const sf::Vector2f&, const sf::Vector2f&)> mapSpawnFunc = std::bind(&GameState::addMapBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_mapController.setSpawnFunction(mapSpawnFunc);

    m_scoreBoard.addObserver(m_players[0]);
    m_scoreBoard.addObserver(m_players[1]);
    m_scoreBoard.addObserver(m_npcController);
    m_scoreBoard.enablePlayer(Category::PlayerOne);

    m_scene.addObserver(m_scoreBoard);
    m_scene.addObserver(m_particleController);
    

    //must be done after controllers are initialised
    Map map("res/maps/testmap2.crm");
    m_players[0].setSpawnPosition(map.getPlayerOneSpawn());
    m_players[1].setSpawnPosition(map.getPlayerTwoSpawn());
    m_npcController.setNpcCount(map.getNpcCount());
    m_scoreBoard.setMaxNpcs(map.getNpcTotal());
    m_mapController.loadMap(map);
    m_scene.setLayerDrawable(m_mapController.getDrawable(), Scene::Solid);
}

bool GameState::update(float dt)
{    
    while (!m_commandStack.empty())
        m_scene.executeCommand(m_commandStack.pop(), dt);

    //update players
    for (auto& p : m_players)
        p.update(dt);

    //update AI
    m_npcController.update(dt);

    //update collision detection
    m_collisionWorld.step(dt);

    //update particles
    m_particleController.update(dt);

    //update water effects
    for (auto& w : waterDrawables)
        w.update(dt);

    //map controller (bonuses etc)
    m_mapController.update(dt);

    m_scene.flush();
    return true;
}

void GameState::draw()
{
    getContext().renderWindow.draw(m_scene);
    getContext().renderWindow.draw(m_particleController);
    getContext().renderWindow.draw(m_scoreBoard);
}

bool GameState::handleEvent(const sf::Event& evt)
{
    switch (evt.type)
    {
    case sf::Event::MouseButtonPressed:
    {  
        auto position = getContext().renderWindow.mapPixelToCoords(sf::Mouse::getPosition(getContext().renderWindow));
        switch (evt.mouseButton.button)
        {
        case sf::Mouse::Left:
            addBlock(position, blockSize);
            break;
        case sf::Mouse::Right:
            addNpc(position);
            break;
        default: break;
        }
    break;
    }
    case sf::Event::KeyPressed:
        switch (evt.key.code)
        {
        case sf::Keyboard::Num1:

            break;
        case sf::Keyboard::Num2:
            m_scoreBoard.enablePlayer(Category::PlayerTwo);
            break;
        case sf::Keyboard::P:
            requestStackPush(States::ID::Pause);
            break;
        default:break;
        }
        break;
    default: break;
    }
    
    return true;
}



//private
void GameState::addBlock(const sf::Vector2f& position, const sf::Vector2f& size)
{
    auto blockNode = std::make_unique<Node>("blockNode");
    blockNode->setPosition(position);
    blockNode->setDrawable(&blockShape);
    blockNode->setCategory(Category::Block);
    blockNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Block, size));
    blockNode->addObserver(m_players[0]);
    blockNode->addObserver(m_players[1]);
    blockNode->addObserver(m_scoreBoard);
    m_scene.addNode(blockNode, Scene::Dynamic);
}

void GameState::addPlayer(const sf::Vector2f& position, Player& player)
{
    if (player.canSpawn())
    {
        auto playerNode = std::make_unique<Node>("Player");
        playerNode->setPosition(position);
        playerNode->setDrawable(&playerShape);
        playerNode->setCategory(player.getType());
        playerNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Player, playerShape.getSize()));
        playerNode->addObserver(player);
        playerNode->addObserver(m_npcController);
        playerNode->addObserver(m_scoreBoard);
        playerNode->addObserver(m_particleController);
        m_scene.addNode(playerNode, Scene::Dynamic);

        player.setSpawnable(false);
        player.setSize(playerShape.getSize());
    }
}

void GameState::addNpc(const sf::Vector2f& position)
{
    auto npcNode = std::make_unique<Node>("npc");
    npcNode->setPosition(position);
    npcNode->setDrawable(&npcShape);
    npcNode->setCategory(Category::Npc);
    npcNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Npc, npcShape.getSize()));
    npcNode->addObserver(m_npcController);
    npcNode->addObserver(m_scoreBoard);
    npcNode->addObserver(m_particleController);
    m_scene.addNode(npcNode, Scene::Dynamic);
}

void GameState::addMapBody(Category::Type type, const sf::Vector2f& position, const sf::Vector2f& size)
{
    switch (type)
    {
    case Category::Block:
        addBlock(position, size);
        break;
    case Category::Solid:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(position);
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Solid, size));
        m_scene.addNode(node, Scene::Solid);
    }
        break;
    case Category::Water:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(position);
        waterDrawables.emplace_back(size);
        node->setDrawable(&waterDrawables.back());
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Water, size));
        node->addObserver(m_particleController);
        m_scene.addNode(node, Scene::Water);
    }
        break;
    case Category::Item:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(position);
        node->setCategory(Category::Item);
        //TODO this is still temp so we may end up cloning shapes instead of reusing them
        //eventually the item controllers will manage drawable resources
        shapes.emplace_back(solidShape);
        shapes.back().setSize(size);
        shapes.back().setOutlineColor(sf::Color::Yellow);
        node->setDrawable(&shapes.back());
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Item, size));
        node->addObserver(m_particleController);
        //TODO add scoreboard observer so we can add points
        m_scene.addNode(node, Scene::Dynamic);
    }
        break;

    default: break;
    }
}