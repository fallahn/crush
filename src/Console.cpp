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

#include <Console.hpp>
#include <Util.hpp>
#include <FileSystem.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <fstream>

namespace
{
    const sf::Uint8 maxBufferLines = 42u;
    const sf::Uint8 maxBufferLength = 100u;
    const sf::Vector2f size(1920.f, 768.f);
    const float textOffset = 10.f;
    const std::string prompt(">");
    const std::string cursor("_");
    const sf::Uint16 charSize = 25u;
    const sf::Uint8 commandHistorySize = 10u;
    const std::string configExtension(".con");
}

Console::Console(const sf::Font& font)
    : m_historyIndex    (0u),
    m_show              (false),
    m_cheatsEnabled     (true)
{
    //setup drawables
    m_bufferText.setFont(font);
    m_bufferText.setCharacterSize(charSize);
    m_bufferText.move(textOffset, size.y - (charSize * 2u));

    m_outputText.setFont(font);
    m_outputText.setCharacterSize(charSize);
    m_outputText.move(textOffset, textOffset);

    m_backgroundRect.setFillColor(sf::Color(0u, 0u, 0u, 100u));
    m_backgroundRect.setSize(size);
    
    updateText();
    mapStrToInput();

    //create default commands

    //---list commands---//
    CommandData cd;
    cd.action = [this](CommandList l)->std::string
    {
        for (const auto& i : m_items)
            print(i.first + ": " + i.second.help + ((i.second.flags & CommandFlag::Cheat) ? " CHEAT" : ""));
        return std::to_string(m_items.size()) + " commands found";
    };
    cd.help = "lists all available commands";
    addItem("list_commands", cd);

    //---show help for command---//
    cd.action = [this](CommandList l)->std::string
    {
        if (l.empty())
        {
            exec("list_commands");
            return "Help usage: type help <command> to display any available information for that command.";
        }

        const auto i = m_items.find(l[0]);
        if (i != m_items.end())
        {
            std::string info = i->second.help;
            if (info.empty()) info = "No help available.";
            return i->first + ": " + info;
        }
        else
        {
            return i->first + ": command not found.";
        }
    };
    cd.help = "displays any available help information for given command";
    addItem("help", cd);

    //---bind command to key or action---//
    cd.action = [this](CommandList l)->std::string
    {
        return bindInput(l);
    };
    cd.help = "params <input> <command> bind console command to input event such as key or mouse button presses";
    addItem("bind", cd);

    //TODO unbind inputs / commands

    //---output list of available commands---//
    cd.action = [this](CommandList l)->std::string
    {
        std::ofstream file("commands.txt");
        for (const auto& i : m_items)
            file << i.first + ": " + i.second.help << ((i.second.flags & CommandFlag::Cheat) ? " CHEAT" : "") << std::endl;
        file << std::endl;
        file << "Inputs available for mapping:" << std::endl;
        file << "---------------------------" << std::endl;
        for (const auto& k : m_keyMap)
            file << "Key: " << k.first << std::endl;
        file << std::endl;

        for (const auto& m : m_mouseMap)
            file << "Mouse: " << m.first << std::endl;
        file << std::endl;

        for (const auto& j : m_joyMap)
            file << "Controller: " << j.first << std::endl;
        file << std::endl;

        for (const auto& j : m_axisMap)
            file << "Controller axis: " << j.first << std::endl;

        file.close();
        return "Wrote file commands.txt";
    };
    cd.help = "creates a text file containing all currently registered commands.";
    addItem("create_cmd_list", cd);

    //---executes a given config file if available---//
    cd.action = [this](CommandList l)->std::string
    {
        if (l.empty())
        {
            return "No file specified. Usage: <string> filename";
        }
        std::string fileName = l[0];
        if (FileSystem::getFileExtension(fileName) != configExtension)
            fileName += configExtension;
        std::fstream file(fileName);
        if (file.fail())
        {
            return "Could not open " + fileName;
        }
        else
        {
            std::string line;
            while (std::getline(file, line))
            {
                exec(line);
            }
            return "Finished executing " + fileName;
        }
    };
    cd.help = "param <string> executes specified .con file";
    addItem("exec", cd);

    //---echos a string to the console output---//
    cd.action = [this](CommandList l)->std::string
    {
        if (l.empty()) return " echo Usage: <string> prints a string to the console";
        print(l[0]);
        return "";
    };
    cd.help = "param <string> prints a string to the console window";
    addItem("echo", cd);

    //---exports config list to given config file---//
    cd.action = [this](CommandList l)->std::string
    {
        std::string fileName;
        if (!l.empty())
        {
            fileName = l[0];
            if (fileName.substr(fileName.size() - 4) != configExtension)
            {
                fileName += configExtension;
            }
        }
        else
        {
            fileName = "default.con";
        }
        std::ofstream cfgFile(fileName);
        for (const auto& cmd : m_configList)
            cfgFile << cmd << std::endl;
        return "Wrote " + std::to_string(m_configList.size()) + " lines to " + fileName;
    };
    cd.help = "<path (optional)> saves the current configuration to configuration file";
    addItem("export_config", cd);

    //---enables commands which require the cheat flag set---//
    cd.action = [this](CommandList l)->std::string
    {
        m_cheatsEnabled = !m_cheatsEnabled;
        return "set cheats enabled to " + (m_cheatsEnabled) ? "true" : "false";
    };
    cd.help = "toggles enabling cheat commands";
    addItem("cheats_enable", cd);
}

//public
void Console::addItem(const std::string& name, CommandData cd)
{
    assert(m_items.find(name) == m_items.end());
    m_items.insert(std::make_pair(name, cd));
}

void Console::removeItem(const std::string& name)
{
    const auto item = m_items.find(name);
    if (item != m_items.end())
        m_items.erase(item);
}

void Console::print(const std::string& text)
{
    m_textBuffer.push_back(text);

    if (m_textBuffer.size() > maxBufferLines)
        m_textBuffer.pop_front();

    if (m_show)
        updateText();
}

bool Console::handleEvent(const sf::Event& e)
{
    //check bindings for command to execute
    //if (!m_show)
    {
        if (e.type == sf::Event::KeyPressed)
        {
            auto result = m_keyBinds.find(e.key.code);
            if (result != m_keyBinds.end()) exec(result->second);
        }
        else if (e.type == sf::Event::JoystickButtonPressed)
        {

        }
        else if (e.type == sf::Event::MouseButtonPressed)
        {

        }
    }
    return false;
}

void Console::handleUIEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
        case sf::Keyboard::BackSpace:
            if (!m_commandBuffer.empty())
                m_commandBuffer.pop_back();
            updateText();
            return;
        case sf::Keyboard::Return:
            parseCommand();
            return;
        case sf::Keyboard::Up:
            if (!m_commandHistory.empty()
                && m_historyIndex > 0)
            {
                m_commandBuffer = m_commandHistory[--m_historyIndex];
                updateText();
            }
            return;
        case sf::Keyboard::Down:
            if (!m_commandHistory.empty()
                && m_historyIndex < m_commandHistory.size())
            {
                m_commandBuffer = m_commandHistory[m_historyIndex++];
                updateText();
            }
            return;
        }
    }
    else if (e.type == sf::Event::TextEntered)
    {
        //handle ASCII characters only, skipping backspace and delete
        if (e.text.unicode > 31
            && e.text.unicode < 127
            && m_commandBuffer.size() < maxBufferLength)
        {
            m_commandBuffer += static_cast<char>(e.text.unicode);
            updateText();

        }
    }
}

void Console::exec(const std::string& cmd)
{
    m_commandBuffer = cmd;
    parseCommand();
}

void Console::show(bool s)
{
    m_show = s;
    if (s) updateText();
}

bool Console::visible() const
{
    return m_show;
}

//private
void Console::addToConfig(const std::string& cmd)
{
    m_configList.push_back(cmd);
    std::sort(m_configList.begin(), m_configList.end());
    m_configList.erase(std::unique(m_configList.begin(), m_configList.end()), m_configList.end());
}

void Console::removeFromConfig(const std::string& cmd)
{
    m_configList.erase(std::remove_if(m_configList.begin(), m_configList.end(),
        [&cmd](const std::string& str)
    {
        std::size_t s = cmd.size();
        auto pos = cmd.find(' ');
        return (pos != std::string::npos) && (s > pos) && (str.substr(0, cmd.size()) == cmd);
    }), m_configList.end());
}

std::string Console::bindInput(CommandList& l)
{
    if (l.empty()) return "bind: parameters <input> <command>";

    std::string input = Util::String::toLower(l[0]);
    const auto s = l.size();
    //output help if any params are missing
    switch (s)
    {
    case 0:
        return "bind: parameters <input> <command>";
    case 1:
    {
        //keyboard
        auto k = m_keyMap.find(input);
        if (k != m_keyMap.end())
        {
            const auto ic = m_keyBinds.find(k->second);
            if (ic != m_keyBinds.end())
            {
                return k->first + " currently bound to: " + ic->second;
            }
        }
        //TODO fall through to test joy/mouse bindings
    }
    return input + " not currently bound to any command.";
    default: break;
    }

    //if params are present attempt to bind
    auto command = m_items.find(l[1]);
    if (command == m_items.end())
        return l[1] + ": command not found.";

    std::string cfgArgs = "bind";
    for (auto& str : l) cfgArgs += " " + str;
    auto k = m_keyMap.find(input);
    if (k != m_keyMap.end())
    {
        //update key if already bound
        auto ic = m_keyBinds.find(k->second);
        if (ic != m_keyBinds.end())
        {
            ic->second = l[1];
            removeFromConfig("bind " + input);
        }
        else
        {
            m_keyBinds.insert(std::make_pair(k->second, l[1]));
        }
        addToConfig(cfgArgs);
        return "bound " + input + " to: '" + l[1] + "'";
    }
    return input + ": invalid input (did you mean to use lower_case?)";
}

void Console::parseCommand()
{
    if (m_commandBuffer.empty()) return;
    //allow commands in quotes as single string
    std::string cmd;
    std::string argString;
    const auto comment = m_commandBuffer.find_first_of('"');
    if (comment != std::string::npos)
    {
        cmd = m_commandBuffer.substr(0u, comment);
        argString = m_commandBuffer.substr(m_commandBuffer.find_first_of('"'), m_commandBuffer.find_last_of('"'));
    }
    else
    {
        cmd = m_commandBuffer;
    }
    //split into command and arguments
    CommandList commandList = Util::String::tokenize(cmd, ' ');
    if (!argString.empty())
    {
        argString.erase(argString.begin());
        argString.erase(argString.end() - 1);
        commandList.push_back(argString);
    }
    if (commandList.empty()) return;

    const auto item = m_items.find(commandList[0]);
    if (item != m_items.end())
    {
        //check if result of command should be saved in con file
        if ((item->second.flags & CommandFlag::Config))
        {
            removeFromConfig(cmd);
            addToConfig(cmd);
        }
        if ((item->second.flags & CommandFlag::Cheat)
            && !m_cheatsEnabled)
        {
            print(commandList[0] + " cannot execute without cheats enabled");
        }
        else if ((item->second.flags & CommandFlag::ConClosed)
            && m_show)
        {
            //don't execute while console open
            
        }
        else
        {
            commandList.erase(commandList.begin()); //remove command name before passing args
            std::string result = item->second.action(commandList);
            if (!result.empty()) print(result);
        }
    }
    else
    {
        //print command not found to console
        print(commandList[0] + ": unknown command.");
    }
    m_commandHistory.push_back(m_commandBuffer);
    if (m_commandHistory.size() > commandHistorySize)
        m_commandHistory.erase(m_commandHistory.begin());
    m_commandBuffer.clear();
    m_historyIndex = m_commandHistory.size();
    updateText();
}

void Console::updateText()
{
    m_bufferText.setString(prompt + m_commandBuffer + cursor);
    //update output buffer
    std::string output;
    for (const auto& s : m_textBuffer)
        output += s + "\n";
    m_outputText.setString(output);
}

void Console::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    rt.draw(m_backgroundRect, states);
    rt.draw(m_outputText, states);
    rt.draw(m_bufferText, states);
}

void Console::mapStrToInput()
{
    m_keyMap["a"] = sf::Keyboard::A;
    m_keyMap["b"] = sf::Keyboard::B;
    m_keyMap["c"] = sf::Keyboard::C;
    m_keyMap["d"] = sf::Keyboard::D;
    m_keyMap["e"] = sf::Keyboard::E;
    m_keyMap["f"] = sf::Keyboard::F;
    m_keyMap["g"] = sf::Keyboard::G;
    m_keyMap["h"] = sf::Keyboard::H;
    m_keyMap["i"] = sf::Keyboard::I;
    m_keyMap["j"] = sf::Keyboard::J;
    m_keyMap["k"] = sf::Keyboard::K;
    m_keyMap["l"] = sf::Keyboard::L;
    m_keyMap["m"] = sf::Keyboard::M;
    m_keyMap["n"] = sf::Keyboard::N;
    m_keyMap["o"] = sf::Keyboard::O;
    m_keyMap["p"] = sf::Keyboard::P;
    m_keyMap["q"] = sf::Keyboard::Q;
    m_keyMap["r"] = sf::Keyboard::R;
    m_keyMap["s"] = sf::Keyboard::S;
    m_keyMap["t"] = sf::Keyboard::T;
    m_keyMap["u"] = sf::Keyboard::U;
    m_keyMap["v"] = sf::Keyboard::V;
    m_keyMap["w"] = sf::Keyboard::W;
    m_keyMap["x"] = sf::Keyboard::X;
    m_keyMap["y"] = sf::Keyboard::Y;
    m_keyMap["z"] = sf::Keyboard::Z;
    m_keyMap["num0"] = sf::Keyboard::Num0;
    m_keyMap["num1"] = sf::Keyboard::Num1;
    m_keyMap["num2"] = sf::Keyboard::Num2;
    m_keyMap["num3"] = sf::Keyboard::Num3;
    m_keyMap["num4"] = sf::Keyboard::Num4;
    m_keyMap["num5"] = sf::Keyboard::Num5;
    m_keyMap["num6"] = sf::Keyboard::Num6;
    m_keyMap["num7"] = sf::Keyboard::Num7;
    m_keyMap["num8"] = sf::Keyboard::Num8;
    m_keyMap["num9"] = sf::Keyboard::Num9;
    m_keyMap["escape"] = sf::Keyboard::Escape;
    m_keyMap["left_control"] = sf::Keyboard::LControl;
    m_keyMap["left_shift"] = sf::Keyboard::LShift;
    m_keyMap["left_alt"] = sf::Keyboard::LAlt;
    m_keyMap["left_system"] = sf::Keyboard::LSystem;
    m_keyMap["right_control"] = sf::Keyboard::RControl;
    m_keyMap["right_shift"] = sf::Keyboard::RShift;
    m_keyMap["right_alt"] = sf::Keyboard::RAlt;
    m_keyMap["right_System"] = sf::Keyboard::RSystem;
    m_keyMap["menu"] = sf::Keyboard::Menu;
    m_keyMap["left_bracket"] = sf::Keyboard::LBracket;
    m_keyMap["right_bracket"] = sf::Keyboard::RBracket;
    m_keyMap["semicolon"] = sf::Keyboard::SemiColon;
    m_keyMap["comma"] = sf::Keyboard::Comma;
    m_keyMap["period"] = sf::Keyboard::Period;
    m_keyMap["quote"] = sf::Keyboard::Quote;
    m_keyMap["forward_slash"] = sf::Keyboard::Slash;
    m_keyMap["back_slash"] = sf::Keyboard::BackSlash;
    m_keyMap["tilde"] = sf::Keyboard::Tilde;
    m_keyMap["equal"] = sf::Keyboard::Equal;
    m_keyMap["dash"] = sf::Keyboard::Dash;
    m_keyMap["space"] = sf::Keyboard::Space;
    m_keyMap["return"] = sf::Keyboard::Return;
    m_keyMap["backspace"] = sf::Keyboard::BackSpace;
    m_keyMap["tab"] = sf::Keyboard::Tab;
    m_keyMap["pageup"] = sf::Keyboard::PageUp;
    m_keyMap["pagedown"] = sf::Keyboard::PageDown;
    m_keyMap["end"] = sf::Keyboard::End;
    m_keyMap["home"] = sf::Keyboard::Home;
    m_keyMap["insert"] = sf::Keyboard::Insert;
    m_keyMap["delete"] = sf::Keyboard::Delete;
    m_keyMap["add"] = sf::Keyboard::Add;
    m_keyMap["subtract"] = sf::Keyboard::Subtract;
    m_keyMap["multiply"] = sf::Keyboard::Multiply;
    m_keyMap["divide"] = sf::Keyboard::Divide;
    m_keyMap["left"] = sf::Keyboard::Left;
    m_keyMap["right"] = sf::Keyboard::Right;
    m_keyMap["up"] = sf::Keyboard::Up;
    m_keyMap["down"] = sf::Keyboard::Down;
    m_keyMap["numpad0"] = sf::Keyboard::Numpad0;
    m_keyMap["numpad1"] = sf::Keyboard::Numpad1;
    m_keyMap["numpad2"] = sf::Keyboard::Numpad2;
    m_keyMap["numpad3"] = sf::Keyboard::Numpad3;
    m_keyMap["numpad4"] = sf::Keyboard::Numpad4;
    m_keyMap["numpad5"] = sf::Keyboard::Numpad5;
    m_keyMap["numpad6"] = sf::Keyboard::Numpad6;
    m_keyMap["numpad7"] = sf::Keyboard::Numpad7;
    m_keyMap["numpad8"] = sf::Keyboard::Numpad8;
    m_keyMap["numpad9"] = sf::Keyboard::Numpad9;
    m_keyMap["f1"] = sf::Keyboard::F1;
    m_keyMap["f2"] = sf::Keyboard::F2;
    m_keyMap["f3"] = sf::Keyboard::F3;
    m_keyMap["f4"] = sf::Keyboard::F4;
    m_keyMap["f5"] = sf::Keyboard::F5;
    m_keyMap["f6"] = sf::Keyboard::F6;
    m_keyMap["f7"] = sf::Keyboard::F7;
    m_keyMap["f8"] = sf::Keyboard::F8;
    m_keyMap["f9"] = sf::Keyboard::F9;
    m_keyMap["f10"] = sf::Keyboard::F10;
    m_keyMap["f11"] = sf::Keyboard::F11;
    m_keyMap["f12"] = sf::Keyboard::F12;
    m_keyMap["f13"] = sf::Keyboard::F13;
    m_keyMap["f14"] = sf::Keyboard::F14;
    m_keyMap["f15"] = sf::Keyboard::F15;
    m_keyMap["pause"] = sf::Keyboard::Pause;
    ///----mouse-----
    m_mouseMap["mouse_left"] = sf::Mouse::Left;
    m_mouseMap["mouse_right"] = sf::Mouse::Right;
    m_mouseMap["mouse_middle"] = sf::Mouse::Middle;
    m_mouseMap["mouse_x1"] = sf::Mouse::XButton1;
    m_mouseMap["mouse_x2"] = sf::Mouse::XButton2;
    ///----controller----
    m_joyMap["joy_button_0"] = 0;
    m_joyMap["joy_button_1"] = 1;
    m_joyMap["joy_button_2"] = 2;
    m_joyMap["joy_button_3"] = 3;
    m_joyMap["joy_button_4"] = 4;
    m_joyMap["joy_button_5"] = 5;
    m_joyMap["joy_button_6"] = 6;
    m_joyMap["joy_button_7"] = 7;
    m_joyMap["joy_button_8"] = 8;
    m_joyMap["joy_button_9"] = 9;
    m_joyMap["joy_button_10"] = 10;
    m_joyMap["joy_button_11"] = 11;
    m_joyMap["joy_button_12"] = 12;
    m_joyMap["joy_button_13"] = 13;
    m_joyMap["joy_button_14"] = 14;
    m_joyMap["joy_button_15"] = 15;
    m_joyMap["joy_button_16"] = 16;
    m_joyMap["joy_button_17"] = 17;
    m_joyMap["joy_button_18"] = 18;
    m_joyMap["joy_button_19"] = 19;
    m_joyMap["joy_button_20"] = 20;
    m_joyMap["joy_button_21"] = 21;
    m_joyMap["joy_button_22"] = 22;
    m_joyMap["joy_button_23"] = 23;
    m_joyMap["joy_button_24"] = 24;
    m_joyMap["joy_button_25"] = 25;
    m_joyMap["joy_button_26"] = 26;
    m_joyMap["joy_button_27"] = 27;
    m_joyMap["joy_button_28"] = 28;
    m_joyMap["joy_button_29"] = 29;
    m_joyMap["joy_button_30"] = 30;
    m_joyMap["joy_button_31"] = 31;
    m_axisMap["joy_axis_x"] = sf::Joystick::X;
    m_axisMap["joy_axis_y"] = sf::Joystick::Y;
    m_axisMap["joy_axis_z"] = sf::Joystick::Z;
    m_axisMap["joy_axis_r"] = sf::Joystick::R;
    m_axisMap["joy_axis_u"] = sf::Joystick::U;
    m_axisMap["joy_axis_v"] = sf::Joystick::V;
    m_axisMap["joy_pov_x"] = sf::Joystick::PovX;
    m_axisMap["joy_pov_y"] = sf::Joystick::PovY;
}