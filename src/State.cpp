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

#include <State.hpp>
#include <StateStack.hpp>
#include <Resource.hpp>
#include <Game.hpp>
#include <Util.hpp>


#include <SFML/Graphics/RenderWindow.hpp>

State::Context::Context(sf::RenderWindow& window, Game& game, GameData& gd)
    : renderWindow  (window),
    gameInstance    (game),
    gameData        (gd)
{

}


//-------------------------------------------

State::State(StateStack& stateStack, Context context)
    : m_stack           (&stateStack),
    m_context           (context),
    m_loadingSprite     ("res/textures/characters/robot.cra", context.gameInstance.getTextureResource()),
    m_loadingText       ("Loading..", context.gameInstance.getFont("res/fonts/VeraMono.ttf")),
    m_threadRunning     (false),
    m_loadingThread     (&State::updateLoadingScreen, this){}

//protected
void State::requestStackPush(States::ID id)
{
    m_stack->pushState(id);
}

void State::requestStackPop()
{
    m_stack->popState();
}

void State::requestStackClear()
{
    m_stack->clearStates();
}

State::Context State::getContext() const
{
    return m_context;
}

void State::launchLoadingScreen()
{
    m_context.gameInstance.pause();

    m_loadingSprite.setLooped(true);
    m_loadingSprite.play();
    m_loadingSprite.setScale(2.f, 2.f);
    m_loadingSprite.setPosition(m_context.renderWindow.getView().getCenter());
    m_loadingSprite.setOrigin(sf::Vector2f(m_loadingSprite.getFrameSize() / 2));

    Util::Position::centreOrigin(m_loadingText);
    m_loadingText.setPosition(m_loadingSprite.getPosition());
    m_loadingText.move(0.f, 60.f);

    m_context.renderWindow.setActive(false);
    m_threadRunning = true;
    m_loadingThread.launch();
}

void State::quitLoadingScreen()
{
    m_threadRunning = false;
    m_loadingThread.wait();
    m_context.gameInstance.resume();
}

//private
void State::updateLoadingScreen()
{
    while (m_threadRunning)
    {
        m_loadingSprite.update(m_threadClock.restart().asSeconds());
        m_context.renderWindow.clear();
        m_context.renderWindow.draw(m_loadingSprite);
        m_context.renderWindow.draw(m_loadingText);
        m_context.renderWindow.display();
    }
    m_context.renderWindow.setActive(false);
}