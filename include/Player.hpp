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

//responsible for parsing input and assigning the correct commands to the relevant player node

#include <CommandStack.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/System/NonCopyable.hpp>

class Player final// : private sf::NonCopyable
{
public:
    struct Keys
    {
        Keys();
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Keyboard::Key jump;
        sf::Uint8 joyButtonJump;
    };

    Player(CommandStack& commandStack, Category::Type type);
    ~Player() = default;

    void update(float dt);
    void handleEvent(const sf::Event& evt); //TODO should this return bool to mark events have been consumed?

    void setKeyBinds(Keys keys);

private:

    CommandStack& m_commandStack;
    Category::Type m_id;
    sf::Uint8 m_joyId;

    Keys m_keyBinds;
    sf::Uint32 m_buttonMask;
};
