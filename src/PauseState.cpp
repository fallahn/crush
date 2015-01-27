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

#include <PauseState.hpp>
#include <Game.hpp>
#include <Util.hpp>
#include <UIButton.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics//Text.hpp>

namespace
{
    sf::RectangleShape grey;
    sf::Text text;
}

PauseState::PauseState(StateStack& stack, Context context)
    : State             (stack, context),
    m_currentContainer  (Container::Main),
    m_font              (context.gameInstance.getFont("res/fonts/VeraMono.ttf")),
    m_textureResource   (context.gameInstance.getTextureResource())
{
    context.renderWindow.setView(context.defaultView);
    
    grey.setFillColor({ 0u, 0u, 0u, 148u });
    grey.setSize(context.defaultView.getSize());

    text.setFont(m_font);
    text.setString("PAUSED");
    text.setCharacterSize(80u);
    Util::Position::centreOrigin(text);
    text.setPosition(context.defaultView.getCenter());
    text.move(0.f, -270.f);

    context.gameInstance.getMusicPlayer().setPaused(true);

    //build menus
    for (auto i = 0; i < Container::Count; ++i)
        m_uiContainers.emplace_back(m_soundPlayer);

    buildMainMenu();
}

PauseState::~PauseState()
{
    getContext().gameInstance.getMusicPlayer().setPaused(false);
}

bool PauseState::update(float dt)
{
    m_uiContainers[m_currentContainer].update(dt);
    return false;
}

void PauseState::draw()
{
    auto& renderWindow = getContext().renderWindow;
    renderWindow.draw(grey);
    renderWindow.draw(text);
    renderWindow.draw(m_uiContainers[m_currentContainer]);
}

bool PauseState::handleEvent(const sf::Event& evt)
{
    if (evt.type == sf::Event::KeyPressed)
    {
        switch (evt.key.code)
        {
        case sf::Keyboard::P:
            requestStackPop();
            break;
        default: break;
        }
    }
    else if (evt.type == sf::Event::JoystickButtonPressed)
    {
        if (evt.joystickButton.button == 7)
            requestStackPop();
    }

    m_uiContainers[m_currentContainer].handleEvent(evt);

    return false;
}

//private
void PauseState::buildMainMenu()
{
    auto resumeButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    resumeButton->setText("Resume");
    resumeButton->setAlignment(ui::Alignment::Centre);
    resumeButton->setPosition(960.f, 400.f);
    resumeButton->setTextColour(sf::Color::Black);
    resumeButton->setCallback([this]()
    {
        requestStackPop();
    });
    m_uiContainers[Container::Main].addControl(resumeButton);

    auto mainMenuButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    mainMenuButton->setText("Main Menu");
    mainMenuButton->setAlignment(ui::Alignment::Centre);
    mainMenuButton->setPosition(960.f, 450.f);
    mainMenuButton->setTextColour(sf::Color::Black);
    mainMenuButton->setCallback([this]()
    {
        requestStackClear();
        requestStackPush(States::ID::Menu);
    });
    m_uiContainers[Container::Main].addControl(mainMenuButton);

    auto optionsButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    optionsButton->setText("Options");
    optionsButton->setAlignment(ui::Alignment::Centre);
    optionsButton->setTextColour(sf::Color::Black);
    optionsButton->setPosition(960.f, 500.f);
    optionsButton->setCallback([this]()
    {
        //TODO
    });
    m_uiContainers[Container::Main].addControl(optionsButton);

    auto quitButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    quitButton->setText("Quit");
    quitButton->setTextColour(sf::Color::Black);
    quitButton->setAlignment(ui::Alignment::Centre);
    quitButton->setPosition(960.f, 550.f);
    quitButton->setCallback([this]()
    {
        getContext().gameInstance.getConsole().exec("quit");
    });
    m_uiContainers[Container::Main].addControl(quitButton);
}