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

//class for parsing json files describing texture packer atlases.
//not related to animated sprites.

#ifndef SPRITE_SHEET_H_
#define SPRITE_SHEET_H_

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <string>
#include <array>
#include <vector>

class SpriteSheet final
{
public:
    typedef std::array<sf::Vertex, 4u> Quad;

    SpriteSheet(const std::string& path);
    SpriteSheet(const SpriteSheet& copy) = default;
    //SpriteSheet& operator=(const SpriteSheet& s) = delete;
    //SpriteSheet(SpriteSheet&& s){}
    SpriteSheet& operator=(SpriteSheet&&){ return *this; }
    ~SpriteSheet() = default;

    const std::string& getName() const;
    Quad getFrame(const std::string& name, const sf::Vector2f& position);

    sf::Vector2i getFrameSize(sf::Uint8 index) const;
    sf::Uint8 getFrameCount() const;

private:
    struct Frame
    {
        Frame() : rotated(false), trimmed(false){}
        std::string filename;
        sf::FloatRect frame;
        bool rotated;
        bool trimmed;
        sf::FloatRect spriteSourceSize;
        sf::Vector2f sourceSize;
        sf::Vector2f pivot;
    };

    std::string m_name;
    std::vector<Frame> m_frames;
};

#endif //SPRITE_SHEET_H_