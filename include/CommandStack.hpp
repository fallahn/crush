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

//creates an aggregated stack of commands, executed by the scene graph each frame

#ifndef COMMAND_STACK_H_
#define COMMAND_STACK_H_

#include <CommandCategories.hpp>

#include <SFML/Config.hpp>

#include <queue>
#include <functional>

class Node;
struct Command
{
    Command();
    ~Command() = default;
    std::function<void(Node&, float)> action;
    sf::Uint32 categoryMask; //target node categories are OR'd into this
};

class CommandStack final
{
public:
    void push(const Command& command);
    Command pop();
    bool empty() const;

private:
    std::queue<Command> m_stack;
};

#endif //COMMAND_STACK_H_