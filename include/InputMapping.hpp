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

//maps input enums to strings and vice versa

#ifndef INPUT_MAP_H_
#define INPUT_MAP_H_

#include <SFML/Window.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <string>

class InputMap final : private sf::NonCopyable
{
public:
    static sf::Keyboard::Key getKey(const std::string&);
    static sf::Mouse::Button getMouseButton(const std::string&);
    static sf::Int16 getJoyButton(const std::string&);
    static sf::Joystick::Axis getAxis(const std::string&);

    static const std::string& toString(sf::Keyboard::Key);
    static const std::string& toString(sf::Mouse::Button);
    static const std::string& toString(sf::Uint16);
    static const std::string& toString(sf::Joystick::Axis);

    static const std::vector<std::string>& getInputList();

    InputMap();
    ~InputMap() = default;

private:


};

#endif //INPUT_MAP_H_
