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

//implements the observer pattern

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <CommandCategories.hpp>

#include <vector>

namespace game
{
    class Event
    {
    public:
        //when a scene node is destroyed
        struct DespawnEvent
        {
            Category::Type type;
        };

        struct PlayerEvent
        {
            Category::Type playerId;
            enum Action
            {
                Landed,
                Jumped,
                Grabbed,
                Died
            }action;
            float positionX, positionY;
        };
    
        //TODO other events such as scoring points

        enum Type
        {
            Despawn,
            Player
        } type;

        union
        {
            DespawnEvent despawn;
            PlayerEvent player;
        };
    };
}

class Subject;
class Observer
{
public:
    virtual ~Observer() = default;
    //when implementing this either deal with event directly, or
    //add to an event list in the inheriting class for deferred handling
    virtual void onNotify(Subject&, const game::Event& evt) = 0;
};

class Subject
{
public:
    virtual ~Subject(){};

    void addObserver(Observer& o)
    {
        m_observers.push_back(&o);
    }

protected:
    void notify(Subject& s, game::Event evt)
    {
        for (auto& o : m_observers)
            o->onNotify(s, evt);
    }

private:
    std::vector<Observer*> m_observers;
};


#endif //OBSERVER_H_