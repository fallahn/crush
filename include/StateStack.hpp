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

//provides state management via a stack model

#ifndef STATESTACK_H_
#define STATESTACK_H_

#include <State.hpp>

#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack final : private sf::NonCopyable
{
public:
    enum class Action
    {
        Push,
        Pop,
        Clear
    };

    explicit StateStack(State::Context context);
    ~StateStack() = default;

    template <typename T>
    void registerState(States::ID id)
    {
        m_factories[id] = [this]()
        {
            return std::make_unique<T>(*this, m_context);
        };
    }

    void update(float dt);
    void draw();
    void handleEvent(const sf::Event& evt);

    void pushState(States::ID id);
    void popState();
    void clearStates();

    bool empty() const;

private:
    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID id = States::ID::None);
        Action action;
        States::ID id;
    };

    std::vector<State::Ptr> m_stack;
    std::vector<PendingChange> m_pendingList;
    State::Context m_context;
    std::map<States::ID, std::function<State::Ptr()>> m_factories;

    State::Ptr createState(States::ID id);
    void applyPendingChanges();

};


#endif//STATESTACK_H_