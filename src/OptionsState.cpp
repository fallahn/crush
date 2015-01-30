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

#include <OptionsState.hpp>
#include <Game.hpp>
#include <Util.hpp>
#include <ContainerPool.hpp>
#include <InputMapping.hpp>
#include <UIButton.hpp>
#include <UISlider.hpp>
#include <UILabel.hpp>
#include <UITextBox.hpp>
#include <UICheckBox.hpp>
#include <UIComboBox.hpp>
#include <UIInputSelect.hpp>

namespace
{
    const sf::Vector2f buttonPrevPosition(400.f, 1000.f);
    const sf::Vector2f buttonNextPosition(1920.f - 400.f, 1000.f);
    const sf::Vector2f buttonOkPosition(960.f, 1000.f);

    const sf::Color containerBackground(100u, 149u, 237u);
}

OptionsState::OptionsState(StateStack& stack, Context context)
    : State             (stack, context),
    m_currentContainer  (Container::InputOptions),
    m_font              (context.gameInstance.getFont("res/fonts/VeraMono.ttf")),
    m_textureResource   (context.gameInstance.getTextureResource())
{
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UIMove, "res/sound/ui/move.wav");
    m_soundPlayer.cacheSound(SoundPlayer::AudioId::UISelect, "res/sound/ui/select.wav");

    for (auto i = 0; i < Container::Count; ++i)
    {
        m_uiContainers.emplace_back(m_soundPlayer);
        m_uiContainers.back().setBackgroundColour(containerBackground);
    }

    buildInputOptions();
    buildSoundOptions();
    buildVideoOptions();
}

//public
bool OptionsState::update(float dt)
{
    m_uiContainers[m_currentContainer].update(dt);
    return false;
}

void OptionsState::draw()
{
    getContext().renderWindow.draw(m_uiContainers[m_currentContainer]);
}

bool OptionsState::handleEvent(const sf::Event& evt)
{
    const auto& rw = getContext().renderWindow;
    auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

    m_uiContainers[m_currentContainer].handleEvent(evt, mousePos);
    return false;
}

//private
void OptionsState::buildInputOptions()
{
    const auto& playerOneKeybinds = getContext().gameData.playerOne.keyBinds;
    ui::InputSelect::Ptr playerOneLeft = std::make_shared<ui::InputSelect>(m_font, "player_one_left");
    playerOneLeft->setPosition(200.f, 200.f);
    playerOneLeft->setValue(InputMap::toString(playerOneKeybinds.left));
    playerOneLeft->setCallback([playerOneLeft, this]()
    {
        std::string str = "set_key ";
        str += playerOneLeft->getValue();
        str += " player_one_left";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerOneLeft);

    ui::InputSelect::Ptr playerOneRight = std::make_shared<ui::InputSelect>(m_font, "player_one_right");
    playerOneRight->setPosition(200.f, 250.f);
    playerOneRight->setValue(InputMap::toString(playerOneKeybinds.right));
    playerOneRight->setCallback([playerOneRight, this]
    {
        std::string str("set_key ");
        str += playerOneRight->getValue();
        str += " player_one_right";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerOneRight);

    ui::InputSelect::Ptr playerOneJump = std::make_shared<ui::InputSelect>(m_font, "player_one_jump");
    playerOneJump->setPosition(200.f, 300.f);
    playerOneJump->setValue(InputMap::toString(playerOneKeybinds.jump));
    playerOneJump->setCallback([playerOneJump, this]()
    {
        std::string str("set_key ");
        str += playerOneJump->getValue();
        str += " player_one_jump";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerOneJump);

    ui::InputSelect::Ptr playerOneDrag = std::make_shared<ui::InputSelect>(m_font, "player_one_drag");
    playerOneDrag->setPosition(200.f, 350.f);
    playerOneDrag->setValue(InputMap::toString(playerOneKeybinds.grab));
    playerOneDrag->setCallback([playerOneDrag, this]()
    {
        std::string str("set_key ");
        str += playerOneDrag->getValue();
        str += " player_one_drag";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerOneDrag);

    ui::InputSelect::Ptr playerOnePickup = std::make_shared<ui::InputSelect>(m_font, "player_one_pickup");
    playerOnePickup->setPosition(200.f, 400.f);
    playerOnePickup->setValue(InputMap::toString(playerOneKeybinds.pickUp));
    playerOnePickup->setCallback([playerOnePickup, this]()
    {
        std::string str("set_key ");
        str += playerOnePickup->getValue();
        str += " player_one_pickup";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerOnePickup);

    //------------------------------------------------------------------

    const auto& playerTwoKeybinds = getContext().gameData.playerTwo.keyBinds;
    ui::InputSelect::Ptr playerTwoLeft = std::make_shared<ui::InputSelect>(m_font, "player_two_left");
    playerTwoLeft->setPosition(1400.f, 200.f);
    playerTwoLeft->setValue(InputMap::toString(playerTwoKeybinds.left));
    playerTwoLeft->setCallback([playerTwoLeft, this]()
    {
        std::string str("set_key ");
        str += playerTwoLeft->getValue();
        str += " player_two_left";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerTwoLeft);

    ui::InputSelect::Ptr playerTwoRight = std::make_shared<ui::InputSelect>(m_font, "player_two_right");
    playerTwoRight->setPosition(1400.f, 250.f);
    playerTwoRight->setValue(InputMap::toString(playerTwoKeybinds.right));
    playerTwoRight->setCallback([playerTwoRight, this]()
    {
        std::string str("set_key ");
        str += playerTwoRight->getValue();
        str += " player_two_right";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerTwoRight);

    ui::InputSelect::Ptr playerTwoJump = std::make_shared<ui::InputSelect>(m_font, "player_two_jump");
    playerTwoJump->setPosition(1400.f, 300.f);
    playerTwoJump->setValue(InputMap::toString(playerTwoKeybinds.jump));
    playerTwoJump->setCallback([playerTwoJump, this]()
    {
        std::string str("set_key ");
        str += playerTwoJump->getValue();
        str += " player_two_jump";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerTwoJump);

    ui::InputSelect::Ptr playerTwoDrag = std::make_shared<ui::InputSelect>(m_font, "player_two_drag");
    playerTwoDrag->setPosition(1400.f, 350.f);
    playerTwoDrag->setValue(InputMap::toString(playerTwoKeybinds.grab));
    playerTwoDrag->setCallback([playerTwoDrag, this]()
    {
        std::string str("set_key ");
        str += playerTwoDrag->getValue();
        str += " player_two_drag";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerTwoDrag);

    ui::InputSelect::Ptr playerTwoPickup = std::make_shared<ui::InputSelect>(m_font, "player_two_pickup");
    playerTwoPickup->setPosition(1400.f, 400.f);
    playerTwoPickup->setValue(InputMap::toString(playerTwoKeybinds.pickUp));
    playerTwoPickup->setCallback([playerTwoPickup, this]()
    {
        std::string str("set_key ");
        str += playerTwoPickup->getValue();
        str += " player_two_pickup";
        getContext().gameInstance.getConsole().exec(str);
    });
    m_uiContainers[Container::InputOptions].addControl(playerTwoPickup);

    ui::Button::Ptr backButton = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    backButton->setPosition(buttonPrevPosition);
    backButton->setAlignment(ui::Alignment::Centre);
    backButton->setTextColour(sf::Color::Black);
    backButton->setText("Back");
    backButton->setCallback([this]()
    {
        //m_currentContainer = Container::Main;
        requestStackPop();
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

void OptionsState::buildSoundOptions()
{
    ui::Slider::Ptr musicVol = std::make_shared<ui::Slider>(m_font, m_textureResource.get("res/textures/ui/slider_handle.png"));
    musicVol->setPosition(960.f, 400.f);
    musicVol->setAlignment(ui::Alignment::Centre);
    musicVol->setText("Music Volume");
    musicVol->setValue(getContext().gameInstance.getMusicPlayer().getVolume());
    musicVol->setCallback([this](const ui::Slider* slider)
    {
        getContext().gameInstance.getConsole().exec("set_music_volume " + std::to_string(slider->getValue()));
    }, ui::Slider::Event::SetInactive);
    musicVol->setCallback([this](const ui::Slider* s)
    {
        getContext().gameInstance.getMusicPlayer().setVolume(s->getValue());
    }, ui::Slider::Event::ValueChanged);
    m_uiContainers[Container::SoundOptions].addControl(musicVol);


    ui::Slider::Ptr fxVol = std::make_shared<ui::Slider>(m_font, m_textureResource.get("res/textures/ui/slider_handle.png"));
    fxVol->setPosition(960.f, 500.f);
    fxVol->setAlignment(ui::Alignment::Centre);
    fxVol->setText("FX Volume");
    fxVol->setValue(SoundPlayer::getVolume());
    fxVol->setCallback([this](const ui::Slider* slider)
    {
        getContext().gameInstance.getConsole().exec("set_sfx_volume " + std::to_string(slider->getValue()));
    }, ui::Slider::Event::SetInactive);
    fxVol->setCallback([this](const ui::Slider* slider)
    {
        SoundPlayer::setVolume(slider->getValue());
    }, ui::Slider::Event::ValueChanged);
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
        m_currentContainer = Container::VideoOptions;
    });
    m_uiContainers[Container::SoundOptions].addControl(buttonNext);
}

void OptionsState::buildVideoOptions()
{
    ui::CheckBox::Ptr fullScreen = std::make_shared<ui::CheckBox>(m_font, m_textureResource.get("res/textures/ui/checkbox.png"));
    fullScreen->setText("Full Screen");
    fullScreen->setAlignment(ui::Alignment::Centre);
    fullScreen->setPosition(960.f, 500.f);
    fullScreen->check(getContext().gameInstance.getVideoSettings().windowStyle == sf::Style::Fullscreen);
    m_uiContainers[Container::VideoOptions].addControl(fullScreen);

    ui::CheckBox::Ptr vsync = std::make_shared<ui::CheckBox>(m_font, m_textureResource.get("res/textures/ui/checkbox.png"));
    vsync->setText("Enable V-Sync");
    vsync->setAlignment(ui::Alignment::Centre);
    vsync->setPosition(960.f, 550.f);
    vsync->check(getContext().gameInstance.getVideoSettings().vSync);
    m_uiContainers[Container::VideoOptions].addControl(vsync);

    ui::ComboBox::Ptr resolution = std::make_shared<ui::ComboBox>(m_font, m_textureResource.get("res/textures/ui/combobox.png"));
    resolution->setAlignment(ui::Alignment::Centre);
    resolution->setPosition(960.f, 600.f);
    const auto& modes = getContext().gameInstance.getVideoSettings().availableVideoModes;
    auto i = 0u;
    auto j = 0u;
    for (const auto& m : modes)
    {
        std::string name = std::to_string(m.width) + " x " + std::to_string(m.height);
        sf::Int32 val = (m.width << 16) | m.height;
        resolution->addItem(name, val);
        //select currently active mode
        if (getContext().gameInstance.getVideoSettings().videoMode != m)
            i++;
        else
            j = i;
    }
    if (i < modes.size()) resolution->setSelectedIndex(j);

    m_uiContainers[Container::VideoOptions].addControl(resolution);

    ui::Button::Ptr buttonPrev = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonPrev->setPosition(buttonPrevPosition);
    buttonPrev->setAlignment(ui::Alignment::Centre);
    buttonPrev->setTextColour(sf::Color::Black);
    buttonPrev->setText("Previous");
    buttonPrev->setCallback([this]()
    {
        m_currentContainer = Container::SoundOptions;
    });
    m_uiContainers[Container::VideoOptions].addControl(buttonPrev);

    ui::Button::Ptr buttonApply = std::make_shared<ui::Button>(m_font, m_textureResource.get("res/textures/ui/button.png"));
    buttonApply->setPosition(buttonNextPosition);
    buttonApply->setAlignment(ui::Alignment::Centre);
    buttonApply->setTextColour(sf::Color::Black);
    buttonApply->setText("Apply");
    buttonApply->setCallback([vsync, fullScreen, resolution, this]()
    {
        //apply settings
        auto& console = getContext().gameInstance.getConsole();
        std::string str = "enable_vsync ";
        str += (vsync->checked()) ? "true" : "false";
        console.exec(str);
        str = "set_fullscreen ";
        str += (fullScreen->checked()) ? "true" : "false";
        console.exec(str);

        sf::Int32 val = resolution->getSelectedValue();
        sf::Int32 width = val >> 16;
        sf::Int32 height = val & 0xFFFF;

        console.exec("set_resolution " + std::to_string(width) + " " + std::to_string(height));
    });
    m_uiContainers[Container::VideoOptions].addControl(buttonApply);
}
