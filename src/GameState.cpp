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
#include <Util.hpp>
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
    Scene::defaultCamera.setView(getContext().defaultView);
    m_scene.addShader(m_shaderResource.get(Shader::Type::FlatShaded));
    m_scene.addShader(m_shaderResource.get(Shader::Type::NormalMapSpecular));
    m_scene.addShader(m_shaderResource.get(Shader::Type::NormalMap));
    m_scene.addShader(m_shaderResource.get(Shader::Type::Metal));
    m_scene.addShader(m_shaderResource.get(Shader::Type::Water));
    m_scene.addShader(m_shaderResource.get(Shader::Type::WaterDrop));

    m_scene.addObserver(m_scoreBoard);
    m_scene.addObserver(m_particleController);
    m_scene.addObserver(m_audioController);

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
    m_players.back().setKeyBinds(context.gameData.playerOne.keyBinds);
    m_players.emplace_back(m_commandStack, Category::PlayerTwo, m_textureResource, m_shaderResource.get(Shader::Type::NormalMapSpecular));
    m_players.back().setKeyBinds(context.gameData.playerTwo.keyBinds);

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

    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::Solid), Scene::Solid);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::RearDetail), Scene::RearDetail);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::FrontDetail), Scene::FrontDetail);
    m_scene.setLayerDrawable(m_mapController.getDrawable(MapController::MapDrawable::Background), Scene::Background);
    m_scene.setAmbientColour(map.getAmbientColour());
    m_scene.setSunLightColour(map.getSunlightColour());

    //sf::Clock c;
    //while (c.getElapsedTime().asSeconds() < 5.f){}

    //close loading screen before starting music
    quitLoadingScreen();
    context.gameInstance.getMusicPlayer().play(music);

    registerConsoleCommands();
    context.renderWindow.setMouseCursorVisible(false);
}

GameState::~GameState()
{
    getContext().renderWindow.setMouseCursorVisible(true);
    unregisterConsoleCommands();
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

    //make sure shader bindings are up to date
    m_shaderResource.updateBindings();
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
        //auto position = getContext().renderWindow.mapPixelToCoords(sf::Mouse::getPosition(getContext().renderWindow));
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
    case sf::Event::JoystickButtonPressed:
        if (evt.joystickButton.button == 7)
            requestStackPush(States::ID::Pause);
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
    m_scene.addNode(blockNode, Scene::DynamicRear);
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
    m_scene.addNode(playerNode, Scene::DynamicRear);

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
        //npcNode->addObserver(*light);
    }*/
    m_scene.addNode(npcNode, Scene::DynamicRear);
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
        }
        node->setBlendMode(sf::BlendAdd);
        m_scene.addNode(node, Scene::DynamicFront);
    }
        break;
    case Category::Light:
    {
        auto node = std::make_unique<Node>();
        node->setCategory(Category::Light);
        //TODO magix0r numb0rz
        auto light = m_scene.addLight(colourToVec3(n.colour), 700.f);
        light->setDepth(50.f);
        node->setLight(light);
        node->setPosition(n.position + (n.size / 2.f));
        node->addObserver(*light);
        node->setDrawable(&lightDrawable);
        node->setBlendMode(sf::BlendAlpha);

        //if (n.anchorOffset) //TODO make not broken (see contraint solve())
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

    case Category::HatDropped:
    {
        auto node = std::make_unique<Node>();
        node->setPosition(n.position);
        node->setDrawable(m_mapController.getDrawable(MapController::MapDrawable::Hat));
        node->setCollisionBody(m_collisionWorld.addBody(CollisionWorld::Body::Type::FreeForm, n.size));
        node->setCategory(n.type);
        auto light = m_scene.addLight(sf::Vector3f(1.f, 0.9f, 0.f), 200.f);
        light->setDepth(50.f);
        node->setLight(light);

        node->addObserver(m_particleController);
        node->addObserver(m_audioController);
        node->addObserver(m_mapController);
        node->addObserver(m_scoreBoard);
        node->addObserver(m_players[0]);
        node->addObserver(m_players[1]);

        m_scene.addNode(node, Scene::DynamicFront);
        break;
    }
    default: break;
    }
}

void GameState::registerConsoleCommands()
{
    auto& console = getContext().gameInstance.getConsole();
    Console::CommandData cd;
    cd.action = [this](Console::CommandList l, sf::Uint32& flags)-> std::string
    {
        if (l.size() < 2)
            return "give: not enough parameters";

        //raise player event with item info and use command stack
        //to target player and raise event from node
        
        Event e;
        e.type = Event::Player;
        e.player.action = Event::PlayerEvent::GotItem;

        if (l[0] != "player_one" && l[0] != "player_two")
            return "unrecognised player";
        
        e.player.playerId = (l[0] == "player_one") ? Category::PlayerOne : Category::PlayerTwo;

        if (l[1] == "extra_life")
        {
            e.player.item = Event::PlayerEvent::Item::ExtraLife;
        }
        else if (l[1] == "extra_speed")
        {
            e.player.item = Event::PlayerEvent::Item::ExtraSpeed;
        }
        else if (l[1] == "jump_increase")
        {
            e.player.item = Event::PlayerEvent::Item::JumpIncrease;
        }
        else if (l[1] == "reverse_controls")
        {
            e.player.item = Event::PlayerEvent::Item::ReverseControls;
        }
        else if (l[1] == "super_strength")
        {
            e.player.item = Event::PlayerEvent::Item::SuperStrength;
        }
        else if (l[1] == "speed_reduction")
        {
            e.player.item = Event::PlayerEvent::Item::SpeedReduction;
        }
        else if (l[1] == "jump_reduction")
        {
            e.player.item = Event::PlayerEvent::Item::JumpReduction;
        }
        else
        {
            return "invalid item. try extra_life, jump_increase, reverse_controls, super_strength, speed_reduction or jump_reduction";
        }        
        
        Command c;
        c.categoryMask |= e.player.playerId;
        c.action = [e](Node& n, float dt)
        {
            Event evt = e;
            assert(n.getCollisionBody());
            auto pos = n.getCollisionBody()->getCentre();
            evt.player.positionX = pos.x;
            evt.player.positionY = pos.y;
            n.raiseEvent(evt);
        };
        m_commandStack.push(c);

        return "";
    };
    cd.help = "params <player> <item> eg player_one super_strength";
    m_consoleCommands.push_back("give");
    console.addItem(m_consoleCommands.back(), cd);

    cd.action = [this](Console::CommandList l, sf::Uint32& flags)->std::string
    {
        if (!l.size()) return "missing parameter: true or false";
        m_npcController.enable((l[0] == "true") ? true : false);
        return "";
    };
    cd.help = "param: true / false";
    m_consoleCommands.push_back("npc_enable");
    console.addItem(m_consoleCommands.back(), cd);
}

void GameState::unregisterConsoleCommands()
{
    for (const auto& c : m_consoleCommands)
        getContext().gameInstance.getConsole().removeItem(c);
}