/*********************************************************************
Matt Marchant 2014
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

//parses map data into object representation

#ifndef MAP_H_
#define MAP_H_

#include <CommandCategories.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

#include <string>
#include <vector>

class Map final
{
public:

    struct Node final
    {
        Node(const std::string& position, const std::string& size, const std::string& type);
        sf::Vector2f position;
        sf::Vector2f size;
        Category::Type type;
    };

    explicit Map(const std::string& path);
    ~Map() = default;

    sf::Uint8 getNpcCount() const;
    sf::Uint8 getNpcTotal() const;
    const std::string& getNpcTexture() const;
    std::string getMapName() const;

    const std::vector<Node>& getNodes() const;

private:

    sf::Uint8 m_npcCount;
    sf::Uint8 m_npcTotal;
    std::string m_npcTexture;

    std::string m_mapName;

    std::vector<Node> m_nodes;
};

#endif //MAP_H_