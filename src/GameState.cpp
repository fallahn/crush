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
#include <Light.hpp>
#include <NormalMapping.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Shader.hpp>

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

    sf::Vector3f colourToVec3(const sf::Color& c)
    {
        return{ static_cast<float>(c.r) / 255.f,
            static_cast<float>(c.g) / 255.f,
            static_cast<float>(c.b) / 255.f };
    }

}

GameState::GameState(StateStack& stack, Context context)
    : State             (stack, context),
    m_textureResource   (context.gameInstance.getTextureResource()),
    m_collisionWorld    (70.f),
    m_npcController     (m_commandStack),
    m_scoreBoard        (stack, context),
    m_particleController(m_textureResource),
    m_mapController     (m_commandStack, m_textureResource, context.gameInstance.getShader(Shader::Type::NormalMap))
{
    //build world
    context.renderWindow.setTitle("Game Screen");
    //context.gameInstance.setClearColour({ 189u, 222u, 237u });
    
    Scene::defaultCamera.setView(getContext().defaultView);

    m_scene.addShader(context.gameInstance.getShader(Shader::Type::NormalMap));
    m_scene.addShader(context.gameInstance.getShader(Shader::Type::Water));

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

    std::function<void(const Map::Node&)> mapSpawnFunc = std::bind(&GameState::addMapBody, this, std::placeholders::_1);
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
    m_scene.setAmbientColour(map.getAmbientColour());
    m_scene.setSunLightColour(map.getSunlightColour());
    context.gameInstance.setClearColour(map.getAmbientColour());
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

    //updates the scene lighting
    m_scene.update(dt);
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

void GameState::addMapBody(const Map::Node& n)
{
    switch (n.type)
    {
    case Category::Block:
        addBlock(n.position, n.size);
        break;
    case Category::Solid:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(n.position);
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Solid, n.size));
        m_scene.addNode(node, Scene::Solid);
    }
        break;
    case Category::Water:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(n.position);
        waterDrawables.emplace_back(getContext().gameInstance.getTextureResource(), getContext().gameInstance.getShader(Shader::Type::Water), n.size);
        node->setDrawable(&waterDrawables.back());
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Water, n.size));
        node->addObserver(m_particleController);
        m_scene.addNode(node, Scene::Water);
    }
        break;
    case Category::Item:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(n.position);
        node->setCategory(Category::Item);
        //TODO this is still temp so we may end up cloning shapes instead of reusing them
        //eventually the item controllers will manage drawable resources
        shapes.emplace_back(solidShape);
        shapes.back().setSize(n.size);
        shapes.back().setOutlineColor(sf::Color::Yellow);
        node->setDrawable(&shapes.back());
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Item, n.size));
        node->addObserver(m_particleController);

        m_scene.addNode(node, Scene::Dynamic);
    }
        break;
    case Category::Light:
    {
        auto node = std::make_unique<Node>();
        node->setCategory(Category::Light);
        //TODO magix0r numb0rz
        auto light = m_scene.addLight(colourToVec3(n.colour), 500.f);
        light->setDepth(150.f);
        node->setLight(light);
        node->setPosition(n.position);
        m_scene.addNode(node);
    }
    default: break;
    }
}