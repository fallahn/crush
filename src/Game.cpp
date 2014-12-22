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

#include <Game.hpp>
#include <TitleState.hpp>
#include <MenuState.hpp>
#include <GameState.hpp>
#include <PauseState.hpp>
#include <GameOverState.hpp>

#include <Resource.hpp>

#include <SFML/Graphics/Font.hpp>

const float Game::m_timePerFrame = 1.f / 60.f;

Game::Game()
    : m_renderWindow(sf::VideoMode(1280, 720), "Crush", sf::Style::Close), //1024, 576
    m_stateStack        (State::Context(m_renderWindow, *this))
{
    registerStates();
    m_stateStack.pushState(States::ID::Title);

    m_renderWindow.setVerticalSyncEnabled(true);
}

//public
void Game::run()
{
    sf::Clock frameClock;
    float timeSinceLastUpdate = 0.f;

    while (m_renderWindow.isOpen())
    {
        float elapsedTime = frameClock.restart().asSeconds();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;

            handleEvents();
            update(m_timePerFrame);
        }

        draw();
    }
}

void Game::setClearColour(sf::Color c)
{
    m_clearColour = c;
}

sf::Font& Game::getFont(const std::string& path)
{
    return m_fontResource.get(path);
}

TextureResource& Game::getTextureResource()
{
    return m_textureResource;
}

sf::Shader& Game::getShader(Shader::Type type)
{
    return m_shaderResource.get(type);
}

//private
void Game::handleEvents()
{
    sf::Event evt;
    while (m_renderWindow.pollEvent(evt))
    {
        //--global esc to quit - TODO remove this--
        if (evt.type == sf::Event::KeyPressed)
            if (evt.key.code == sf::Keyboard::Escape)
                m_renderWindow.close();
        //-----------------------------------------

        m_stateStack.handleEvent(evt);
        
        if(evt.type == sf::Event::Closed)
            m_renderWindow.close();
    }
}

void Game::update(float dt)
{
    m_stateStack.update(dt);
}

void Game::draw()
{
    m_renderWindow.clear(m_clearColour);
    m_stateStack.draw();
    m_renderWindow.display();
}

void Game::registerStates()
{
    m_stateStack.registerState<TitleState>(States::ID::Title);
    m_stateStack.registerState<MenuState>(States::ID::Menu);
    m_stateStack.registerState<GameState>(States::ID::Game);
    m_stateStack.registerState<PauseState>(States::ID::Pause);
    m_stateStack.registerState<GameOverState>(States::ID::GameOver);
}