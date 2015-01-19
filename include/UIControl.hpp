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

//base class for ui controls

#ifndef UI_CONTROL_H_
#define UI_CONTROL_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <memory>

namespace sf
{
    class Event;
}

namespace ui
{
    class Control : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
    {
    public:
        typedef std::shared_ptr<Control> Ptr;

        Control();
        virtual ~Control() = default;

        virtual bool selectable() const = 0;
        bool selected() const;

        virtual void select();
        virtual void deselect();

        virtual bool active() const;
        virtual void activate();
        virtual void deactivate();

        virtual void handleEvent(const sf::Event& e) = 0;
        virtual void update(float dt){};

    private:

        bool m_selected;
        bool m_active;
    };
}

#endif //UI_CONTROL_H_