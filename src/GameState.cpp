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

#include <GameState.hpp>
#include <Game.hpp>
#include <BodyBehaviour.hpp>
#include <Util.hpp>
#include <Particles.hpp>
#include <Map.hpp>
#include <Light.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <iostream>

namespace
{
    sf::CircleShape lightDrawable(6.f);

    const sf::Uint8 maxPlayers = 2u;

    sf::Vector3f colourToVec3(const sf::Color& c)
    {
        return{ static_cast<float>(c.r) / 255.f,
            static_cast<float>(c.g) / 255.f,
            static_cast<float>(c.b) / 255.f };
    }

    const std::string music = "res/sound/music/night_sounds.ogg";


    sf::CircleShape c1(20.f);
    sf::CircleShape c2(10.f);
}

GameState::GameState(StateStack& stack, Context context)
    : State             (stack, context),
    m_textureResource   (context.gameInstance.getTextureResource()),
    m_shaderResource    (context.gameInstance.getShaderResource()),
    m_collisionWorld    (70.f),
    m_npcController     (m_commandStack, m_textureResource, m_shaderResource),
    m_scoreBoard        (stack, context),
    m_particleController(m_textureResource, m_shaderResource),
    m_mapController     (m_commandStack, m_textureResource, m_shaderResource)
{
    //launch loading window
    launchLoadingScreen();
    
    //build world
    context.renderWindow.setTitle("Game Screen");
    
    Scene::defaultCamera.setView(getContext().defaultView);
    m_scene.addShader(m_shaderResource.get(Shader::Type::FlatShaded));
    m_scene.addShader(m_shaderResource.get(Shader::Type::NormalMapSpecular));
    m_scene.addShader(m_shaderResource.get(Shader::Type::NormalMap));
    m_scene.addShader(m_shaderResource.get(Shader::Type::Water));

    float origin = lightDrawable.getRadius();
    lightDrawable.setOrigin(origin, origin);
    lightDrawable.setFillColor(sf::Color(255u, 255u, 255u, 140u));
    lightDrawable.setOutlineColor(sf::Color(255u, 255u, 255u, 11u));
    lightDrawable.setOutlineThickness(80.f);


    //parse map
    Map map("res/maps/" + context.gameData.mapList[context.gameData.mapIndex]);

    //set up controllers
    m_players.reserve(2);
    m_players.emplace_back(m_commandStack, Category::PlayerOne, m_textureResource, m_shaderResource.get(Shader::Type::NormalMapSpecular));
    m_players.emplace_back(m_commandStack, Category::PlayerTwo, m_textureResource, m_shaderResource.get(Shader::Type::NormalMapSpecular));

    std::function<void(const sf::Vector2f&, Player&)> playerSpawnFunc = std::bind(&GameState::addPlayer, this, std::placeholders::_1, std::placeholders::_2);
    m_players[0].setSpawnFunction(playerSpawnFunc);
    m_players[1].setSpawnFunction(playerSpawnFunc);
    m_players[0].setSpawnPosition(map.getPlayerOneSpawn());
    m_players[1].setSpawnPosition(map.getPlayerTwoSpawn());
    //TODO set keybinds from context

    std::function<void(const sf::Vector2f&, const sf::Vector2f&)> npcSpawnFunc = std::bind(&GameState::addNpc, this, std::placeholders::_1, std::placeholders::_2);
    m_npcController.setSpawnFunction(npcSpawnFunc);
    m_npcController.setNpcCount(map.getNpcCount());

    std::function<void(const Map::Node&)> mapSpawnFunc = std::bind(&GameState::addMapBody, this, std::placeholders::_1);
    m_mapController.setSpawnFunction(mapSpawnFunc);
    m_mapController.loadMap(map);

    m_scoreBoard.addObserver(m_players[0]);
    m_scoreBoard.addObserver(m_players[1]);
    m_scoreBoard.addObserver(m_npcController);
    if (context.gameData.playerOne.enabled)
        m_scoreBoard.enablePlayer(Category::PlayerOne);
    if (context.gameData.playerTwo.enabled)
        m_scoreBoard.enablePlayer(Category::PlayerTwo);
    m_scoreBoard.setMaxNpcs(map.getNpcTotal());

    m_scene.addObserver(m_scoreBoard);
    m_scene.addObserver(m_particleController);
    m_scene.addObserver(m_audioController);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::Solid), Scene::Solid);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::RearDetail), Scene::RearDetail);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::FrontDetail), Scene::FrontDetail);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::Background), Scene::Background);
    m_scene.setAmbientColour(map.getAmbientColour());
    m_scene.setSunLightColour(map.getSunlightColour());

    //sf::Clock c;
    //while (c.getElapsedTime().asSeconds() < 5.f){}

    //----temp stuff to test constraint----
    /*auto nodeA = std::make_unique<Node>();
    nodeA->setPosition(960.f, 540.f);
    nodeA->setDrawable(&c1);
    nodeA->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Solid, { c1.getRadius() * 2.f, c1.getRadius()  * 2.f }));

    auto nodeB = std::make_unique<Node>();
    nodeB->setPosition(1160.f, 540.f);
    nodeB->setDrawable(&c1);
    nodeB->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::FreeForm, { 2.f, 2.f }));

    auto nodeC = std::make_unique<Node>();
    nodeC->setPosition(1320.f, 540.f);
    nodeC->setDrawable(&c1);
    nodeC->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::FreeForm, { c1.getRadius() * 2.f, c1.getRadius()  * 2.f }));
    auto lightA = m_scene.addLight(sf::Vector3f(1.f, 0.6f, 0.f), 400.f);
    lightA->setDepth(50.f);
    nodeC->setLight(lightA);

    m_collisionWorld.addConstraint(nodeA->getCollisionBody(), nodeB->getCollisionBody(), 120.f);
    m_collisionWorld.addConstraint(nodeA->getCollisionBody(), nodeC->getCollisionBody(), 120.f);

    m_scene.addNode(nodeA);
    m_scene.addNode(nodeB);
    m_scene.addNode(nodeC);*/

    //-------------------------------------


    //close loading screen before starting music
    quitLoadingScreen();
    context.gameInstance.playMusic(music);
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

    //map controller (bonuses etc)
    m_mapController.update(dt);

    //do sounds
    m_audioController.update();

    //updates the scene lighting
    m_scene.update(dt);

    //update scoreboard
    m_scoreBoard.update(dt);

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
            //addBlock(position, blockSize);
            break;
        case sf::Mouse::Right:
            //addNpc(position);
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
            //m_scoreBoard.enablePlayer(Category::PlayerTwo);
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
    blockNode->setDrawable(m_mapController.getDrawable(MapController::MapDrawable::Block));
    blockNode->setCategory(Category::Block);
    blockNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Block, size));
    blockNode->addObserver(m_players[0]);
    blockNode->addObserver(m_players[1]);
    blockNode->addObserver(m_scoreBoard);
    blockNode->addObserver(m_audioController);
    m_scene.addNode(blockNode, Scene::Dynamic);
}

void GameState::addPlayer(const sf::Vector2f& position, Player& player)
{
    auto playerNode = std::make_unique<Node>("Player");
    playerNode->setPosition(position);
    playerNode->setDrawable(player.getSprite());
    playerNode->setCategory(player.getType());
    playerNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Player, player.getSize()));
    playerNode->addObserver(player);
    playerNode->addObserver(m_npcController);
    playerNode->addObserver(m_scoreBoard);
    playerNode->addObserver(m_particleController);
    playerNode->addObserver(m_audioController);
    m_scene.addNode(playerNode, Scene::Dynamic);

    player.setSpawnable(false);
}

void GameState::addNpc(const sf::Vector2f& position, const sf::Vector2f& size)
{
    auto npcNode = std::make_unique<Node>();
    npcNode->setPosition(position);
    npcNode->setDrawable(m_npcController.getDrawable());
    npcNode->setCategory(Category::Npc);
    npcNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Npc, size));
    npcNode->addObserver(m_npcController);
    npcNode->addObserver(m_scoreBoard);
    npcNode->addObserver(m_particleController);
    npcNode->addObserver(m_audioController);
    
   /* auto light = m_scene.addLight(sf::Vector3f(1.f, 0.5f, 0.f), 200.f);
    if (light)
    {
        light->setDepth(50.f);
        npcNode->setLight(light);
        npcNode->addObserver(*light);
    }*/
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
        auto drawable = static_cast<WaterDrawable*>(m_mapController.getDrawable(MapController::MapDrawable::Water));
        drawable->setSize(n.size);
        node->setDrawable(drawable);
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
        node->setDrawable(m_mapController.getDrawable(MapController::MapDrawable::Item));
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Item, n.size));
        node->addObserver(m_particleController);
        node->addObserver(m_audioController);

        auto light = m_scene.addLight(sf::Vector3f(0.34f, 0.96f, 1.f), 400.f);
        if (light)
        {
            light->setDepth(50.f);
            node->setLight(light);
            node->addObserver(*light);
        }

        m_scene.addNode(node, Scene::Dynamic);
    }
        break;
    case Category::Light:
    {
        auto node = std::make_unique<Node>();
        node->setCategory(Category::Light);
        //TODO magix0r numb0rz
        auto light = m_scene.addLight(colourToVec3(n.colour), 200.f);
        light->setDepth(50.f);
        node->setLight(light);
        node->setPosition(n.position + (n.size / 2.f));
        node->addObserver(*light);
        node->setDrawable(&lightDrawable);
        node->setBlendMode(sf::BlendAlpha);

        //if (n.anchorOffset)
        //{
        //    //we want a constraint on this light
        //    node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::FreeForm, { lightDrawable.getRadius(), lightDrawable.getRadius() }));

        //    auto anchorNode = std::make_unique<Node>();
        //    anchorNode->setPosition(n.position);
        //    anchorNode->move(0.f, -n.anchorOffset);
        //    anchorNode->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::Anchor, { lightDrawable.getRadius(), lightDrawable.getRadius() }));
        //    anchorNode->setDrawable(&c1);

        //    m_collisionWorld.addConstraint(anchorNode->getCollisionBody(), node->getCollisionBody(), n.anchorOffset);
        //    m_scene.addNode(anchorNode);
        //}

        m_scene.addNode(node, Scene::Background);
        break;
    }

    case Category::Hat:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(n.position);
        node->setDrawable(m_mapController.getDrawable(MapController::MapDrawable::Hat));
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::FreeForm, n.size));
        auto light = m_scene.addLight(sf::Vector3f(1.f, 0.9f, 0.f), 400.f);
        light->setDepth(50.f);
        node->setLight(light);
        //node->setBlendMode(sf::BlendAdd);
        m_scene.addNode(node, Scene::FrontDetail);
        break;
    }
    default: break;
    }
}