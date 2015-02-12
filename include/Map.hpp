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

//parses map data into object representation

#ifndef MAP_H_
#define MAP_H_

#include <CommandCategories.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Scene.hpp>

#include <string>
#include <vector>

class Map final
{
public:

    struct Node final
    {
        Node();
        Node(const std::string& position, const std::string& size, const std::string& type, const sf::Color& colour, const std::string& layer);
        sf::Vector2f position;
        sf::Vector2f size;
        Category::Type type;
        sf::Color colour;
        Scene::Layer layer;
        std::string spriteSheet;
        std::string image;
        float anchorOffset;
    };

    explicit Map(const std::string& path);
    ~Map() = default;

    sf::Uint8 getNpcCount() const;
    sf::Uint8 getNpcTotal() const;
    const std::string& getNpcTexture() const;
    std::string getMapName() const;
    const sf::Color& getAmbientColour() const;
    const sf::Color& getSunlightColour() const;

    const std::vector<Node>& getNodes() const;

    const sf::Vector2f& getPlayerOneSpawn() const;
    const sf::Vector2f& getPlayerTwoSpawn() const;

    const std::string& getBackgroundImageName() const;
    const std::string& getPlatformImageName() const;

    const std::string& getAudioTheme() const;

private:

    sf::Vector2f m_playerOneSpawn;
    sf::Vector2f m_playerTwoSpawn;

    sf::Uint8 m_npcCount;
    sf::Uint8 m_npcTotal;
    std::string m_npcTexture;

    std::string m_mapName;
    sf::Color m_ambientColour;
    sf::Color m_sunlightColour;

    std::string m_backgroundImageName;
    std::string m_platformImageName;

    std::string m_audioTheme;

    std::vector<Node> m_nodes;
};

#endif //MAP_H_