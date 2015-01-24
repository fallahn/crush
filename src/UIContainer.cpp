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

#include <UIContainer.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace ui;

namespace
{
    const float deadzone = 40.f;
}

Container::Container(SoundPlayer& sp)
    : m_selectedIndex   (-1),
    m_soundPlayer       (sp)
{}

//public
void Container::addControl(Control::Ptr c)
{
    m_controls.push_back(c);

    if (!hasSelection() && c->selectable())
        select(m_controls.size() - 1);
}

bool Container::selectable() const
{
    return false;
}

void Container::handleEvent(const sf::Event& e)
{
    //pass event to selected control
    if (hasSelection() && m_controls[m_selectedIndex]->active())
    {
        m_controls[m_selectedIndex]->handleEvent(e);
    }
    //keyboard input
    else if (e.type == sf::Event::KeyReleased)
    {
        if (e.key.code == sf::Keyboard::Up
            || e.key.code == sf::Keyboard::Left)
        {
            selectPrevious();
        }
        else if (e.key.code == sf::Keyboard::Down
            || e.key.code == sf::Keyboard::Right)
        {
            selectNext();
        }
        else if (e.key.code == sf::Keyboard::Return/* ||
            e.key.code == sf::Keyboard::Space*/)
        {
            if (hasSelection())
            {
                m_controls[m_selectedIndex]->activate();
                m_soundPlayer.play(SoundPlayer::AudioId::UISelect);
            }
        }
    }
    //controller navigation
    else if (e.type == sf::Event::JoystickMoved)
    {
        if (e.joystickMove.axis == sf::Joystick::Axis::PovY)
        {
            if (e.joystickMove.position > deadzone)
            {
                selectPrevious();
            }
            else if (e.joystickMove.position < -deadzone)
            {
                selectNext();
            }
        }//axis is inverse so needs its own branch :/
        else if (e.joystickMove.axis == sf::Joystick::Axis::PovX)
        {
            if (e.joystickMove.position > deadzone)
            {
                selectNext();
            }
            else if (e.joystickMove.position < -deadzone)
            {
                selectPrevious();
            }
        }
    }
    //controller button
    else if (e.type == sf::Event::JoystickButtonReleased)
    {
        //TODO check button mapping (A and START on xbawx)
        if (e.joystickButton.button == 0 || e.joystickButton.button == 7)
        {
            if (hasSelection())
            {
                m_controls[m_selectedIndex]->activate();
                m_soundPlayer.play(SoundPlayer::AudioId::UISelect);
            }
        }
    }
}

//private
bool Container::hasSelection() const
{
    return (m_selectedIndex >= 0);
}

void Container::select(sf::Int16 index)
{
    if (m_controls[index]->selectable())
    {
        if (hasSelection())
            m_controls[m_selectedIndex]->deselect();

        m_controls[index]->select();
        m_selectedIndex = index;

        m_soundPlayer.play(SoundPlayer::AudioId::UIMove);
    }
}

void Container::selectNext()
{
    if (!hasSelection()) return;

    auto next = m_selectedIndex;

    do
    {
        next = (next + 1) % m_controls.size();
    } while (!m_controls[next]->selectable());

    select(next);
}

void Container::selectPrevious()
{
    if (!hasSelection()) return;

    auto prev = m_selectedIndex;

    do
    {
        prev = (prev + m_controls.size() - 1) % m_controls.size();
    } while (!m_controls[prev]->selectable());

    select(prev);
}

void Container::update(float dt)
{
    for (auto& c : m_controls)
        c->update(dt);
}

void Container::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto& c : m_controls)
    {
        rt.draw(*c, states);
    }
}