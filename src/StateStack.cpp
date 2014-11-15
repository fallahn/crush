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

#include <StateStack.hpp>

#include <cassert>

StateStack::StateStack(State::Context context)
    : m_context(context){}

//public
void StateStack::update(float dt)
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); ++i)
    {
        if (!(*i)->update(dt)) break;
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    for (auto& state : m_stack)
        state->draw();
}

void StateStack::handleEvent(const sf::Event& evt)
{
    for (auto i = m_stack.rbegin(); i != m_stack.rend(); ++i)
    {
        if (!(*i)->handleEvent(evt)) break;
    }
    applyPendingChanges();
}

void StateStack::pushState(States::ID id)
{
    m_pendingList.push_back(PendingChange(Action::Push, id));
}

void StateStack::popState()
{
    m_pendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
    m_pendingList.push_back(PendingChange(Action::Clear));
}

State::Ptr StateStack::createState(States::ID id)
{
    auto result = m_factories.find(id);
    assert(result != m_factories.end());

    return result->second();
}

//private
void StateStack::applyPendingChanges()
{
    for (auto& change : m_pendingList)
    {
        switch (change.action)
        {
        case Action::Push:
            m_stack.push_back(createState(change.id));
            break;
        case Action::Pop:
            m_stack.pop_back();
            break;
        case Action::Clear:
            m_stack.clear();
            break;
        default: break;
        }
    }
    m_pendingList.clear();
}

//-----------------------------------------------

StateStack::PendingChange::PendingChange(Action action, States::ID id)
    : action    (action),
    id          (id){}