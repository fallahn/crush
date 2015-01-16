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

//ABC for game states

#ifndef STATE_H_
#define STATE_H_

#include <StateIds.hpp>
#include <AnimatedSprite.hpp>
#include <Player.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include <atomic>

namespace sf
{
    class RenderWindow;
}

struct GameData
{
    std::vector<std::string> mapList;
    sf::Uint16 mapIndex; //current map in the list

    GameData() : mapIndex(0u){}

    struct PlayerData
    {
        sf::Uint32 score;
        float hatTime;
        sf::Int16 lives;
        std::string name;
        Player::Keys keyBinds;
        bool enabled;
        bool hasHat;

        PlayerData() : score(0u), hatTime(0.f), lives(5), name("Number Six"), enabled(true), hasHat(false){}
    }playerOne, playerTwo;
};

class StateStack;
class Game;
class TextureResource;
class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(sf::RenderWindow& renderWindow, Game& game, GameData& gd);
        sf::RenderWindow& renderWindow;
        Game& gameInstance;
        sf::View defaultView; //automatically updated to correctly letterbox screen
        GameData& gameData;
    };

    State(StateStack& stateStack, Context context);
    virtual ~State() = default;

    virtual bool handleEvent(const sf::Event& evt) = 0;
    virtual bool update(float dt) = 0;
    virtual void draw() = 0;

protected:
    void requestStackPush(States::ID id);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

    void launchLoadingScreen();    
    void quitLoadingScreen();

private:

    StateStack* m_stack;
    Context m_context;

    AnimatedSprite m_loadingSprite;
    sf::Text m_loadingText;
    std::atomic<bool> m_threadRunning;
    sf::Thread m_loadingThread;
    sf::Clock m_threadClock;
    void updateLoadingScreen();
};

#endif //STATE_H_