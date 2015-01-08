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

//provides in game command console

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>
#include <string>
#include <vector>
#include <list>

class Console final : public sf::Drawable, private sf::NonCopyable
{
public:
    enum CommandFlag
    {
        None      = 0,
        Cheat     = (1 << 0), //command will only execute if cheats enabled
        Config    = (1 << 1), //command should be saved to .con file
        ConClosed = (1 << 2) //only executes if console is closed
    };

    typedef std::vector<std::string> CommandList;
    typedef std::function<std::string(CommandList&)> Command;

    struct CommandData
    {
        CommandData() : flags(0u){}
        Command action;
        std::string help;
        sf::Uint32 flags;
    };

    explicit Console(const sf::Font& font);
    ~Console() = default;

    void addItem(const std::string& name, CommandData cd);
    void removeItem(const std::string& name);
    void print(const std::string& text);

    bool handleEvent(const sf::Event& e);
    void handleUIEvent(const sf::Event& e);
    void exec(const std::string& cmd);

    void show(bool shown = true);
    bool visible() const;

private:

    std::string m_commandBuffer; //console input
    CommandList m_commandHistory;
    sf::Uint8 m_historyIndex;
    std::list<std::string> m_textBuffer; //console output
    std::map<std::string, CommandData> m_items;

    CommandList m_configList; //list of commands to store in autoexec
    void addToConfig(const std::string& cmd);
    void removeFromConfig(const std::string& cmd);

    bool m_show;
    bool m_cheatsEnabled;
    sf::RectangleShape m_backgroundRect;
    sf::Text m_outputText;
    sf::Text m_bufferText;

    std::map<std::string, sf::Keyboard::Key> m_keyMap;
    std::map<std::string, sf::Mouse::Button> m_mouseMap;
    std::map<std::string, sf::Uint16> m_joyMap;
    std::map<std::string, sf::Joystick::Axis> m_axisMap;

    std::map<sf::Keyboard::Key, std::string> m_keyBinds;
    std::string bindInput(CommandList& l);

    void parseCommand();
    void updateText();
    void mapStrToInput();
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //CONSOLE_H_