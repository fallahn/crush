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

#include <InputMapping.hpp>

#include <map>
#include <cassert>

namespace
{
    std::map<std::string, sf::Keyboard::Key> keyMap;
    std::map<std::string, sf::Mouse::Button> mouseMap;
    std::map<std::string, sf::Int16> joyMap;
    std::map<std::string, sf::Joystick::Axis> axisMap;

    InputMap mapInstance;
}

InputMap::InputMap()
{
    keyMap["a"] = sf::Keyboard::A;
    keyMap["b"] = sf::Keyboard::B;
    keyMap["c"] = sf::Keyboard::C;
    keyMap["d"] = sf::Keyboard::D;
    keyMap["e"] = sf::Keyboard::E;
    keyMap["f"] = sf::Keyboard::F;
    keyMap["g"] = sf::Keyboard::G;
    keyMap["h"] = sf::Keyboard::H;
    keyMap["i"] = sf::Keyboard::I;
    keyMap["j"] = sf::Keyboard::J;
    keyMap["k"] = sf::Keyboard::K;
    keyMap["l"] = sf::Keyboard::L;
    keyMap["m"] = sf::Keyboard::M;
    keyMap["n"] = sf::Keyboard::N;
    keyMap["o"] = sf::Keyboard::O;
    keyMap["p"] = sf::Keyboard::P;
    keyMap["q"] = sf::Keyboard::Q;
    keyMap["r"] = sf::Keyboard::R;
    keyMap["s"] = sf::Keyboard::S;
    keyMap["t"] = sf::Keyboard::T;
    keyMap["u"] = sf::Keyboard::U;
    keyMap["v"] = sf::Keyboard::V;
    keyMap["w"] = sf::Keyboard::W;
    keyMap["x"] = sf::Keyboard::X;
    keyMap["y"] = sf::Keyboard::Y;
    keyMap["z"] = sf::Keyboard::Z;
    keyMap["num0"] = sf::Keyboard::Num0;
    keyMap["num1"] = sf::Keyboard::Num1;
    keyMap["num2"] = sf::Keyboard::Num2;
    keyMap["num3"] = sf::Keyboard::Num3;
    keyMap["num4"] = sf::Keyboard::Num4;
    keyMap["num5"] = sf::Keyboard::Num5;
    keyMap["num6"] = sf::Keyboard::Num6;
    keyMap["num7"] = sf::Keyboard::Num7;
    keyMap["num8"] = sf::Keyboard::Num8;
    keyMap["num9"] = sf::Keyboard::Num9;
    keyMap["escape"] = sf::Keyboard::Escape;
    keyMap["left_control"] = sf::Keyboard::LControl;
    keyMap["left_shift"] = sf::Keyboard::LShift;
    keyMap["left_alt"] = sf::Keyboard::LAlt;
    keyMap["left_system"] = sf::Keyboard::LSystem;
    keyMap["right_control"] = sf::Keyboard::RControl;
    keyMap["right_shift"] = sf::Keyboard::RShift;
    keyMap["right_alt"] = sf::Keyboard::RAlt;
    keyMap["right_System"] = sf::Keyboard::RSystem;
    keyMap["menu"] = sf::Keyboard::Menu;
    keyMap["left_bracket"] = sf::Keyboard::LBracket;
    keyMap["right_bracket"] = sf::Keyboard::RBracket;
    keyMap["semicolon"] = sf::Keyboard::SemiColon;
    keyMap["comma"] = sf::Keyboard::Comma;
    keyMap["period"] = sf::Keyboard::Period;
    keyMap["quote"] = sf::Keyboard::Quote;
    keyMap["forward_slash"] = sf::Keyboard::Slash;
    keyMap["back_slash"] = sf::Keyboard::BackSlash;
    keyMap["tilde"] = sf::Keyboard::Tilde;
    keyMap["equal"] = sf::Keyboard::Equal;
    keyMap["dash"] = sf::Keyboard::Dash;
    keyMap["space"] = sf::Keyboard::Space;
    keyMap["return"] = sf::Keyboard::Return;
    keyMap["backspace"] = sf::Keyboard::BackSpace;
    keyMap["tab"] = sf::Keyboard::Tab;
    keyMap["pageup"] = sf::Keyboard::PageUp;
    keyMap["pagedown"] = sf::Keyboard::PageDown;
    keyMap["end"] = sf::Keyboard::End;
    keyMap["home"] = sf::Keyboard::Home;
    keyMap["insert"] = sf::Keyboard::Insert;
    keyMap["delete"] = sf::Keyboard::Delete;
    keyMap["add"] = sf::Keyboard::Add;
    keyMap["subtract"] = sf::Keyboard::Subtract;
    keyMap["multiply"] = sf::Keyboard::Multiply;
    keyMap["divide"] = sf::Keyboard::Divide;
    keyMap["left"] = sf::Keyboard::Left;
    keyMap["right"] = sf::Keyboard::Right;
    keyMap["up"] = sf::Keyboard::Up;
    keyMap["down"] = sf::Keyboard::Down;
    keyMap["numpad0"] = sf::Keyboard::Numpad0;
    keyMap["numpad1"] = sf::Keyboard::Numpad1;
    keyMap["numpad2"] = sf::Keyboard::Numpad2;
    keyMap["numpad3"] = sf::Keyboard::Numpad3;
    keyMap["numpad4"] = sf::Keyboard::Numpad4;
    keyMap["numpad5"] = sf::Keyboard::Numpad5;
    keyMap["numpad6"] = sf::Keyboard::Numpad6;
    keyMap["numpad7"] = sf::Keyboard::Numpad7;
    keyMap["numpad8"] = sf::Keyboard::Numpad8;
    keyMap["numpad9"] = sf::Keyboard::Numpad9;
    keyMap["f1"] = sf::Keyboard::F1;
    keyMap["f2"] = sf::Keyboard::F2;
    keyMap["f3"] = sf::Keyboard::F3;
    keyMap["f4"] = sf::Keyboard::F4;
    keyMap["f5"] = sf::Keyboard::F5;
    keyMap["f6"] = sf::Keyboard::F6;
    keyMap["f7"] = sf::Keyboard::F7;
    keyMap["f8"] = sf::Keyboard::F8;
    keyMap["f9"] = sf::Keyboard::F9;
    keyMap["f10"] = sf::Keyboard::F10;
    keyMap["f11"] = sf::Keyboard::F11;
    keyMap["f12"] = sf::Keyboard::F12;
    keyMap["f13"] = sf::Keyboard::F13;
    keyMap["f14"] = sf::Keyboard::F14;
    keyMap["f15"] = sf::Keyboard::F15;
    keyMap["pause"] = sf::Keyboard::Pause;
    keyMap["unknown"] = sf::Keyboard::Unknown;
    //----mouse-----
    mouseMap["mouse_left"] = sf::Mouse::Left;
    mouseMap["mouse_right"] = sf::Mouse::Right;
    mouseMap["mouse_middle"] = sf::Mouse::Middle;
    mouseMap["mouse_x1"] = sf::Mouse::XButton1;
    mouseMap["mouse_x2"] = sf::Mouse::XButton2;
    ///----controller----
    joyMap["joy_button_0"] = 0;
    joyMap["joy_button_1"] = 1;
    joyMap["joy_button_2"] = 2;
    joyMap["joy_button_3"] = 3;
    joyMap["joy_button_4"] = 4;
    joyMap["joy_button_5"] = 5;
    joyMap["joy_button_6"] = 6;
    joyMap["joy_button_7"] = 7;
    joyMap["joy_button_8"] = 8;
    joyMap["joy_button_9"] = 9;
    joyMap["joy_button_10"] = 10;
    joyMap["joy_button_11"] = 11;
    joyMap["joy_button_12"] = 12;
    joyMap["joy_button_13"] = 13;
    joyMap["joy_button_14"] = 14;
    joyMap["joy_button_15"] = 15;
    joyMap["joy_button_16"] = 16;
    joyMap["joy_button_17"] = 17;
    joyMap["joy_button_18"] = 18;
    joyMap["joy_button_19"] = 19;
    joyMap["joy_button_20"] = 20;
    joyMap["joy_button_21"] = 21;
    joyMap["joy_button_22"] = 22;
    joyMap["joy_button_23"] = 23;
    joyMap["joy_button_24"] = 24;
    joyMap["joy_button_25"] = 25;
    joyMap["joy_button_26"] = 26;
    joyMap["joy_button_27"] = 27;
    joyMap["joy_button_28"] = 28;
    joyMap["joy_button_29"] = 29;
    joyMap["joy_button_30"] = 30;
    joyMap["joy_button_31"] = 31;

    axisMap["joy_axis_x"] = sf::Joystick::X;
    axisMap["joy_axis_y"] = sf::Joystick::Y;
    axisMap["joy_axis_z"] = sf::Joystick::Z;
    axisMap["joy_axis_r"] = sf::Joystick::R;
    axisMap["joy_axis_u"] = sf::Joystick::U;
    axisMap["joy_axis_v"] = sf::Joystick::V;
    axisMap["joy_pov_x"] = sf::Joystick::PovX;
    axisMap["joy_pov_y"] = sf::Joystick::PovY;
}

sf::Keyboard::Key InputMap::getKey(const std::string& str)
{
    auto r = keyMap.find(str);
    return (r != keyMap.end()) ? r->second : sf::Keyboard::Unknown;
}

sf::Mouse::Button InputMap::getMouseButton(const std::string& str)
{
    auto r = mouseMap.find(str);
    assert(r != mouseMap.end());
    return r->second;
}

sf::Int16 InputMap::getJoyButton(const std::string& str)
{
    auto r = joyMap.find(str);
    return (r != joyMap.end()) ? r->second : -1;
}

sf::Joystick::Axis InputMap::getAxis(const std::string& str)
{
    auto r = axisMap.find(str);
    assert(r != axisMap.end());
    return r->second;
}

//---------------------

const std::string& InputMap::toString(sf::Keyboard::Key k)
{
    auto r = std::find_if(keyMap.begin(), keyMap.end(),
        [k](const std::map<std::string, sf::Keyboard::Key>::value_type& v)
    {
        return (v.second == k);
    });
    assert(r != keyMap.end());
    return r->first;
}

const std::string& InputMap::toString(sf::Mouse::Button b)
{
    auto r = std::find_if(mouseMap.begin(), mouseMap.end(),
        [b](const std::map<std::string, sf::Mouse::Button>::value_type& v)
    {
        return (v.second == b);
    });
    assert(r != mouseMap.end());
    return r->first;
}

const std::string& InputMap::toString(sf::Uint16 b)
{
    auto r = std::find_if(joyMap.begin(), joyMap.end(),
        [b](const std::map<std::string, sf::Int16>::value_type& v)
    {
        return (v.second == b);
    });
    assert(r != joyMap.end());
    return r->first;
}

const std::string& InputMap::toString(sf::Joystick::Axis a)
{
    auto r = std::find_if(axisMap.begin(), axisMap.end(),
        [a](const std::map<std::string, sf::Joystick::Axis>::value_type& v)
    {
        return (v.second == a);
    });
    assert(r != axisMap.end());
    return r->first;
}