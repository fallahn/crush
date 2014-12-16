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

#include <Map.hpp>
#include <Util.hpp>

#include <picojson.h>

#include <cassert>
#include <fstream>

#include <iostream>

Map::Map(const std::string& path)
    : m_npcCount    (0u),
    m_npcTotal      (0u)
{
    //attempt to parse json file
    std::ifstream file(path);
    assert(file.good());

    //json should be a single line
    std::string jsonString;
    //file >> jsonString;
    std::getline(file, jsonString);
    assert(!jsonString.empty());

    picojson::value v;
    auto err = picojson::parse(v, jsonString);
    if (!err.empty())
    {
        std::cerr << "Map Parse: " << err << std::endl;
        return;
    }

    //map data
    if (v.get("NpcCount").is<double>())
        m_npcCount = static_cast<sf::Uint8>(v.get("NpcCount").get<double>());
    else
        std::cerr << "Map Parse: missing NpcCount value." << std::endl;

    if (v.get("NpcTotal").is<double>())
        m_npcTotal = static_cast<sf::Uint8>(v.get("NpcTotal").get<double>());
    else
        std::cerr << "Map Parse: missing NpcTotal value." << std::endl;

    if (v.get("MapName").is<std::string>())
        m_mapName = v.get("MapName").get<std::string>();
    else
        std::cerr << "Map Parse: missing MapName value." << std::endl;
    //---
    //TODO load texture info
    //---
    if (v.get("PlayerOneSpawn").is<std::string>())
        m_playerOneSpawn = Util::Vector::vec2FromString(v.get("PlayerOneSpawn").get<std::string>());
    else
        std::cerr << "Map Parse: missing Player One spawn position." << std::endl;


    if (v.get("PlayerTwoSpawn").is<std::string>())
        m_playerTwoSpawn = Util::Vector::vec2FromString(v.get("PlayerTwoSpawn").get<std::string>());
    else
        std::cerr << "Map Parse: missing Player Two spawn position." << std::endl;

    //node array
    if (v.get("Nodes").is<picojson::array>())
    {
        auto nodes = v.get("Nodes").get<picojson::array>();
        for (const auto& n : nodes)
        {
            if (n.get("Position").is<std::string>()
                && n.get("Size").is<std::string>()
                && n.get("Type").is<std::string>())
            {
                m_nodes.emplace_back(n.get("Position").get<std::string>(),
                    n.get("Size").get<std::string>(),
                    n.get("Type").get<std::string>());
            }
            else
            {
                std::cerr << "Map Parse: node has missing or corrupt data..." << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Map Parse: missing or corrupt node data array." << std::endl;
    }
}

//public
sf::Uint8 Map::getNpcCount() const
{
    return m_npcCount;
}

sf::Uint8 Map::getNpcTotal() const
{
    return m_npcTotal;
}

const std::string& Map::getNpcTexture() const
{
    return m_npcTexture;
}

std::string Map::getMapName() const
{
    auto pos = m_mapName.find_last_of('\\');
    assert(pos != std::string::npos);
    return m_mapName.substr(pos);
}

const std::vector<Map::Node>& Map::getNodes() const
{
    return m_nodes;
}

const sf::Vector2f& Map::getPlayerOneSpawn() const
{
    return m_playerOneSpawn;
}

const sf::Vector2f& Map::getPlayerTwoSpawn() const
{
    return m_playerTwoSpawn;
}

//node ctor
Map::Node::Node(const std::string& position, const std::string& size, const std::string& type)
    : position  (Util::Vector::vec2FromString(position)),
    size        (Util::Vector::vec2FromString(size)),
    type        (Category::None)
{
    if (type == "Block")
    {
        this->type = Category::Block;
    }
    else if (type == "Solid")
    {
        this->type = Category::Solid;
    }
    else if (type == "PlayerOne")
    {
        this->type = Category::PlayerOne;
    }
    else if (type == "PlayerTwo")
    {
        this->type = Category::PlayerTwo;
    }
    else if (type == "Bonus")
    {
        this->type = Category::Bonus;
    }
    else if (type == "ExtraLife")
    {
        this->type = Category::ExtraLife;
    }
    else if (type == "Water")
    {
        this->type = Category::Water;
    }
}