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

#include <MenuState.hpp>
#include <Game.hpp>
#include <Util.hpp>
#include <InputMapping.hpp>
#include <UIButton.hpp>
#include <UISlider.hpp>
#include <UILabel.hpp>
#include <UITextBox.hpp>
#include <UICheckBox.hpp>
#include <UIComboBox.hpp>
#include <UIInputSelect.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace
{
    std::string helpStr =
        "HOW TO PLAY:\n"
        "\n"
        "Use WAD to move Player One and Arrow keys\n"
        "to move Player Two. Hold Space (Player One)\n"
        "or Right Shift (Player Two) to grab a box and\n"
        "drag it around. Press S (Player One) or Down\n"
        "(Player Two) to pick up a block and stack it\n"
        "on others (also works with a controller!).\n"
        "Score the most points by crushing the bad guys\n"
        "(or each other!). Crush enemies between blocks..\n"
        "but don't accidentally crush yourself!\n";

    const std::string music = "res/sound/music/intro.ogg";

    const sf::Vector2f buttonPrevPosition(400.f, 1000.f);
    const sf::Vector2f buttonNextPosition(1920.f - 400.f, 1000.f);
    const sf::Vector2f buttonOkPosition(960.f, 1000.f);
}

MenuState::MenuState(StateStack& stack, Context context)
    : State             (stack, context),
    m_currentContainer  (Container::Main),
    m_font              (context.gameInstance.getFont("res/fonts/VeraMono.ttf")),
    m_textureResource   (context.gameInstance.getTextureResource())
{
    context.renderWindow.setView(context.defaultView);

    //reset game data
    /*context.gameData.playerOne = {};
    context.gameData.playerOne.name = "Sidney Burnsides";
    context.gameData.playerTwo = {};
    context.gameData.playerTwo.name = "Gemima Headson";*/
    //TODO reset just scores
    context.gameData.mapIndex = 0;
    
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UIMove, "res/sound/ui/move.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UISelect, "res/sound/ui/select.wav");

    for (auto i = 0; i < Container::Count; ++i)
        m_uiContainers.emplace_back(m_soundPlayer);

    buildMainMenu();
    buildInputOptions();
    buildSoundOptions();
    buildGraphicsOptions();
    buildNameInput();
    buildHelp();

    context.gameInstance.playMusic(music);
}

void MenuState::draw()
{
    getContext().renderWindow.draw(m_uiContainers[m_currentContainer]);
}

bool MenuState::update(float dt)
{
    m_uiContainers[m_currentContainer].update(dt);
    return true;
}

bool MenuState::handleEvent(const sf::Event& evt)
{
    m_uiContainers[m_currentContainer].handleEvent(evt);
    return true;
}

//private
void MenuState::buildMainMenu()
{
    //player one button
    auto playerOneButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    playerOneButton->setText("One Player");
    playerOneButton->setPosition(960.f, 200.f);
    playerOneButton->setAlignment(ui::Alignment::Centre);
    playerOneButton->setTextColour(sf::Color::Black);
    playerOneButton->setCallback([this]()
    {
        getContext().gameData.playerTwo.enabled = false;
        m_currentContainer = Container::NameInput;
    });
    m_uiContainers[Container::Main].addControl(playerOneButton);

    //player two button
    auto playerTwoButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    playerTwoButton->setText("Two Player");
    playerTwoButton->setPosition(960.f, 248.f);
    playerTwoButton->setAlignment(ui::Alignment::Centre);
    playerTwoButton->setTextColour(sf::Color::Black);
    playerTwoButton->setCallback([this]()
    {
        m_currentContainer = Container::NameInput;
    });
    m_uiContainers[Container::Main].addControl(playerTwoButton);

    //options button
    auto optionsButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    optionsButton->setText("Options");
    optionsButton->setTextColour(sf::Color::Black);
    optionsButton->setPosition(960.f, 296.f);
    optionsButton->setAlignment(ui::Alignment::Centre);
    optionsButton->setCallback([this]()
    {
        m_currentContainer = Container::InputOptions;
    });
    m_uiContainers[Container::Main].addControl(optionsButton);

    //help button
    auto helpButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    helpButton->setText("How To Play");
    helpButton->setTextColour(sf::Color::Black);
    helpButton->setPosition(960.f, 344.f);
    helpButton->setAlignment(ui::Alignment::Centre);
    helpButton->setCallback([this]()
    {
        m_currentContainer = Container::Help;
    });
    m_uiContainers[Container::Main].addControl(helpButton);
}

void MenuState::buildInputOptions()
{
    ui::InputSelect::Ptr playerOneLeft = std::make_shared<ui::InputSelect>(m_font, "player_one_left");
    playerOneLeft->setPosition(200.f, 200.f);
    playerOneLeft->setValue(InputMap::toString(getContext().gameData.playerOne.keyBinds.left));
    //playerOneLeft->setCallback([&, this](){getContext().gameInstance.getConsole().exec("set_key player_one_left " + getValue()); });
    m_uiContainers[Container::InputOptions].addControl(playerOneLeft);

    ui::InputSelect::Ptr playerOneRight = std::make_shared<ui::InputSelect>(m_font, "player_one_right");
    playerOneRight->setPosition(200.f, 250.f);
    playerOneRight->setValue(InputMap::toString(getContext().gameData.playerOne.keyBinds.right));
    m_uiContainers[Container::InputOptions].addControl(playerOneRight);

    ui::InputSelect::Ptr playerOneJump = std::make_shared<ui::InputSelect>(m_font, "player_one_jump");
    playerOneJump->setPosition(200.f, 300.f);
    playerOneJump->setValue(InputMap::toString(getContext().gameData.playerOne.keyBinds.jump));
    m_uiContainers[Container::InputOptions].addControl(playerOneJump);

    ui::InputSelect::Ptr playerOneDrag = std::make_shared<ui::InputSelect>(m_font, "player_one_drag");
    playerOneDrag->setPosition(200.f, 350.f);
    playerOneDrag->setValue(InputMap::toString(getContext().gameData.playerOne.keyBinds.grab));
    m_uiContainers[Container::InputOptions].addControl(playerOneDrag);

    ui::InputSelect::Ptr playerOnePickup = std::make_shared<ui::InputSelect>(m_font, "player_one_pickup");
    playerOnePickup->setPosition(200.f, 400.f);
    playerOnePickup->setValue(InputMap::toString(getContext().gameData.playerOne.keyBinds.pickUp));
    m_uiContainers[Container::InputOptions].addControl(playerOnePickup);

    //------------------------------------------------------------------

    ui::InputSelect::Ptr playerTwoLeft = std::make_shared<ui::InputSelect>(m_font, "player_two_left");
    playerTwoLeft->setPosition(1400.f, 200.f);
    playerTwoLeft->setValue("left"); //TODO parse context data
    m_uiContainers[Container::InputOptions].addControl(playerTwoLeft);

    ui::InputSelect::Ptr playerTwoRight = std::make_shared<ui::InputSelect>(m_font, "player_two_right");
    playerTwoRight->setPosition(1400.f, 250.f);
    playerTwoRight->setValue("right");
    m_uiContainers[Container::InputOptions].addControl(playerTwoRight);

    ui::InputSelect::Ptr playerTwoJump = std::make_shared<ui::InputSelect>(m_font, "player_two_jump");
    playerTwoJump->setPosition(1400.f, 300.f);
    playerTwoJump->setValue("up");
    m_uiContainers[Container::InputOptions].addControl(playerTwoJump);

    ui::InputSelect::Ptr playerTwoDrag = std::make_shared<ui::InputSelect>(m_font, "player_two_drag");
    playerTwoDrag->setPosition(1400.f, 350.f);
    playerTwoDrag->setValue("right_shift");
    m_uiContainers[Container::InputOptions].addControl(playerTwoDrag);

    ui::InputSelect::Ptr playerTwoPickup = std::make_shared<ui::InputSelect>(m_font, "player_two_pickup");
    playerTwoPickup->setPosition(1400.f, 400.f);
    playerTwoPickup->setValue("down");
    m_uiContainers[Container::InputOptions].addControl(playerTwoPickup);

    ui::Button::Ptr backButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    backButton->setPosition(buttonPrevPosition);
    backButton->setAlignment(ui::Alignment::Centre);
    backButton->setTextColour(sf::Color::Black);
    backButton->setText("Back");
    backButton->setCallback([this]()
    {
        m_currentContainer = Container::Main;
    });
    m_uiContainers[Container::InputOptions].addControl(backButton);

    ui::Button::Ptr nextButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    nextButton->setPosition(buttonNextPosition);
    nextButton->setAlignment(ui::Alignment::Centre);
    nextButton->setTextColour(sf::Color::Black);
    nextButton->setText("Next");
    nextButton->setCallback([this]()
    {
        m_currentContainer = Container::SoundOptions;
    });
    m_uiContainers[Container::InputOptions].addControl(nextButton);

}

void MenuState::buildSoundOptions()
{
    //TODO load volume from config
    ui::Slider::Ptr musicVol = std::make_shared<ui::Slider>(m_font, m_textureResource.get("res/textures/ui/slider_handle.png"));
    musicVol->setPosition(960.f, 400.f);
    musicVol->setAlignment(ui::Alignment::Centre);
    musicVol->setText("Music Volume");
    m_uiContainers[Container::SoundOptions].addControl(musicVol);

    ui::Slider::Ptr fxVol = std::make_shared<ui::Slider>(m_font, m_textureResource.get("res/textures/ui/slider_handle.png"));
    fxVol->setPosition(960.f, 500.f);
    fxVol->setAlignment(ui::Alignment::Centre);
    fxVol->setText("FX Volume");
    m_uiContainers[Container::SoundOptions].addControl(fxVol);

    ui::Button::Ptr buttonPrev = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonPrev->setPosition(buttonPrevPosition);
    buttonPrev->setAlignment(ui::Alignment::Centre);
    buttonPrev->setTextColour(sf::Color::Black);
    buttonPrev->setText("Previous");
    buttonPrev->setCallback([this]()
    {
        m_currentContainer = Container::InputOptions;
    });
    m_uiContainers[Container::SoundOptions].addControl(buttonPrev);

    ui::Button::Ptr buttonNext = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonNext->setPosition(buttonNextPosition);
    buttonNext->setAlignment(ui::Alignment::Centre);
    buttonNext->setTextColour(sf::Color::Black);
    buttonNext->setText("Next");
    buttonNext->setCallback([this]()
    {
        m_currentContainer = Container::GraphicsOptions;
    });
    m_uiContainers[Container::SoundOptions].addControl(buttonNext);
}

void MenuState::buildGraphicsOptions()
{
    ui::CheckBox::Ptr fullScreen = std::make_shared<ui::CheckBox>(m_font, m_textureResource.get("res/textures/ui/checkbox.png"));
    fullScreen->setText("Full Screen");
    fullScreen->setAlignment(ui::Alignment::Centre);
    fullScreen->setPosition(960.f, 500.f);
    m_uiContainers[Container::GraphicsOptions].addControl(fullScreen);

    ui::CheckBox::Ptr vsync = std::make_shared<ui::CheckBox>(m_font, m_textureResource.get("res/textures/ui/checkbox.png"));
    vsync->setText("Enable V-Sync");
    vsync->setAlignment(ui::Alignment::Centre);
    vsync->setPosition(960.f, 550.f);
    m_uiContainers[Container::GraphicsOptions].addControl(vsync);
    
    ui::ComboBox::Ptr resolution = std::make_shared<ui::ComboBox>(m_font, m_textureResource.get("res/textures/ui/combobox.png"));
    resolution->setAlignment(ui::Alignment::Centre);
    resolution->setPosition(960.f, 600.f);
    resolution->addItem("funt", 3);
    resolution->addItem("McBunt", 23);
    resolution->addItem("Speef", 93475);
    m_uiContainers[Container::GraphicsOptions].addControl(resolution);

    ui::Button::Ptr buttonPrev = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonPrev->setPosition(buttonPrevPosition);
    buttonPrev->setAlignment(ui::Alignment::Centre);
    buttonPrev->setTextColour(sf::Color::Black);
    buttonPrev->setText("Previous");
    buttonPrev->setCallback([this]()
    {
        m_currentContainer = Container::SoundOptions;
    });
    m_uiContainers[Container::GraphicsOptions].addControl(buttonPrev);

    ui::Button::Ptr buttonApply = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonApply->setPosition(buttonNextPosition);
    buttonApply->setAlignment(ui::Alignment::Centre);
    buttonApply->setTextColour(sf::Color::Black);
    buttonApply->setText("Apply");
    buttonApply->setCallback([this]()
    {
        //TODO recreate window
    });
    m_uiContainers[Container::GraphicsOptions].addControl(buttonApply);
}

void MenuState::buildNameInput()
{
    ui::TextBox::Ptr playerOneInput = std::make_shared<ui::TextBox>(m_font);
    playerOneInput->setPosition(960.f, 200.f);
    playerOneInput->setAlignment(ui::Alignment::Centre);
    playerOneInput->setText(getContext().gameData.playerOne.name);
    m_uiContainers[Container::NameInput].addControl(playerOneInput);

    ui::TextBox::Ptr playerTwoInput = std::make_shared<ui::TextBox>(m_font);
    playerTwoInput->setPosition(960.f, 260.f);
    playerTwoInput->setAlignment(ui::Alignment::Centre);
    playerTwoInput->setText(getContext().gameData.playerTwo.name);
    m_uiContainers[Container::NameInput].addControl(playerTwoInput);
    
    ui::Button::Ptr buttonPrev = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonPrev->setPosition(buttonPrevPosition);
    buttonPrev->setAlignment(ui::Alignment::Centre);
    buttonPrev->setTextColour(sf::Color::Black);
    buttonPrev->setText("Back");
    buttonPrev->setCallback([this]()
    {
        m_currentContainer = Container::Main;
    });
    m_uiContainers[Container::NameInput].addControl(buttonPrev);

    ui::Button::Ptr buttonStart = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonStart->setPosition(buttonNextPosition);
    buttonStart->setAlignment(ui::Alignment::Centre);
    buttonStart->setTextColour(sf::Color::Black);
    buttonStart->setText("BEGIN!");
    buttonStart->setCallback([this]()
    {
        //TODO update context data with names
        
        //start game
        requestStackPop();
        requestStackPush(States::ID::Game);
    });
    m_uiContainers[Container::NameInput].addControl(buttonStart);
}

void MenuState::buildHelp()
{
    ui::Label::Ptr helpText = std::make_shared<ui::Label>(helpStr, m_font);
    helpText->setFontSize(60u);
    helpText->setPosition(60.f, 40.f);
    m_uiContainers[Container::Help].addControl(helpText);

    ui::Button::Ptr okButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    okButton->setPosition(buttonOkPosition);
    okButton->setAlignment(ui::Alignment::Centre);
    okButton->setTextColour(sf::Color::Black);
    okButton->setText("OK");
    okButton->setCallback([this]()
    {
        m_currentContainer = Container::Main;
    });
    m_uiContainers[Container::Help].addControl(okButton);
}