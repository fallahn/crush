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

//observes the game to update the scores

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <Observer.hpp>
#include <State.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

#include <list>


class ScoreBoard final : public Observer, public Subject, public sf::Drawable, private sf::NonCopyable
{
public:
    ScoreBoard(StateStack& stack, State::Context context);
    ~ScoreBoard() = default;

    void update(float dt);

    void onNotify(Subject& s, const Event& evt) override;

    void enablePlayer(Category::Type player);
    void setMaxNpcs(sf::Uint8 count);

private:
    StateStack& m_stack;
    State::Context m_context;

    sf::Int16 m_playerOneLives;
    sf::Int16 m_playerTwoLives;

    sf::Uint32 m_playerOneScore;
    sf::Uint32 m_playerTwoScore;

    sf::Uint16 m_playerOneKillStreak;
    sf::Uint16 m_playerTwoKillStreak;

    float* m_playerOneHatTime;
    float* m_playerTwoHatTime;
    float m_nullHatTime;
    float* m_hatTimer;

    bool m_playerOneExtinct;
    bool m_playerTwoExtinct;

    sf::Uint8 m_maxNpcs;
    sf::Uint8 m_spawnedNpcs;
    sf::Uint8 m_deadNpcs;

    void updateText(Category::Type type);
    void updateGameData();
    void disablePlayer(Category::Type player);
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

    void killstreakMessage();

    class Message final : public sf::Drawable
    {
    public:
        Message(const std::string& text, const sf::Vector2f& position, const sf::Font& font, bool zoom = false);
        Message(const Message& copy) = default;
        ~Message() = default;

        void update(float dt);

        void setColour(const sf::Color& c);
        bool stopped() const;

    private:

        sf::Color m_colour;
        float m_transparency;
        bool m_zoom;
        sf::Text m_text;
        float m_messageSpeed;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };

    std::list<Message> m_messages;
};

#endif //HUD_H_