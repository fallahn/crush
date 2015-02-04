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

#include <SpriteSheet.hpp>
#include <Util.hpp>
#include <JsonUtil.hpp>

#include <SFML/Graphics/Transform.hpp>

#include <picojson.h>

#include <cassert>
#include <fstream>

SpriteSheet::SpriteSheet(const std::string& path)
{
    std::ifstream file(path);
    assert(file.good());
    assert(Util::File::validLength(file));

    //read the entire file into memory first
    std::string jsonString;
    while (!file.eof())
    {
        std::string temp;
        file >> temp;
        jsonString += temp;
    }
    assert(!jsonString.empty());
    file.close();

    picojson::value pv;
    auto err = picojson::parse(pv, jsonString);
    if (err.empty())
    {
        if (pv.get("meta").get("image").is<std::string>())
        {
            m_name = pv.get("meta").get("image").get<std::string>();
        }
        else
        {
            std::cerr << "Sprite Sheet: Missing image name." << std::endl;
        }

        //load frame info
        if (pv.get("frames").is<picojson::array>())
        {
            const auto& frames = pv.get("frames").get<picojson::array>();
            for (const auto& f : frames)
            {
                m_frames.emplace_back();
                auto& frame = m_frames.back();

                if (f.get("filename").is<std::string>())
                    frame.filename = f.get("filename").get<std::string>();

                if (f.get("frame").is<picojson::object>())
                {
                    const auto& o = f.get("frame").get<picojson::object>();
                    Util::Json::parseJsonObject(o, frame.frame);
                }

                if (f.get("rotated").is<bool>())
                {
                    frame.rotated = f.get("rotated").get<bool>();
                }

                if (f.get("trimmed").is<bool>())
                {
                    frame.trimmed = f.get("trimmed").get<bool>();
                }

                if (f.get("spriteSourceSize").is<picojson::object>())
                {
                    const auto& o = f.get("spriteSourceSize").get<picojson::object>();
                    Util::Json::parseJsonObject(o, frame.spriteSourceSize);
                }

                if (f.get("sourceSize").is<picojson::object>())
                {
                    const auto& o = f.get("sourceSize").get<picojson::object>();
                    Util::Json::parseJsonObject(o, frame.sourceSize);
                }

                if (f.get("pivot").is<picojson::object>())
                {
                    const auto& o = f.get("pivot").get<picojson::object>();
                    Util::Json::parseJsonObject(o, frame.pivot);
                }
            }
        }
        else
        {
            std::cerr << "Sprite Sheet: " << m_name << ", missing frames array." << std::endl;
        }
    }
    else
    {
        std::cerr << "Sprite Sheet: " << err << std::endl;
    }
}

const std::string& SpriteSheet::getName() const
{
    return m_name;
}

SpriteSheet::Quad SpriteSheet::getFrame(const std::string& name, const sf::Vector2f& position)
{
    //TODO cache quads as frame members so if they are requested more than once they don't need to be recreated?
    //have to make sure not to cache any previous positions

    auto result = std::find_if(m_frames.begin(), m_frames.end(),
        [&name](const Frame& f)
    {
        return (f.filename == name); 
    });

    Quad q = {};
    if (result != m_frames.end())
    {       
        auto frame = result->frame;        
        if (result->rotated) //swap w/h
        {
            auto temp = frame.width;
            frame.width = frame.height;
            frame.height = temp;
        }

        std::vector<sf::Vector2f> positions = 
        {
            { 0.f, 0.f },
            { frame.width, 0.f },
            { frame.width, frame.height },
            { 0.f, frame.height }
        };

        if (result->rotated)
        {
            sf::Transform t;
            //pivot coords are normalised
            t.rotate(-90.f, { frame.width * result->pivot.x, frame.height * result->pivot.y });

            for (auto& p : positions)
            {
                p = t.transformPoint(p);
                p.y += frame.width; //moves origin
            }
        }

        std::vector<sf::Vector2f> coords = 
        {
            { frame.left, frame.top },
            { frame.left + frame.width, frame.top },
            { frame.left + frame.width, frame.top + frame.height },
            { frame.left, frame.top + frame.height }
        };

        for (auto i = 0u; i < q.size(); ++i)
        {
            q[i].texCoords = coords[i];
            q[i].position = positions[i] + position;
        }
    }
    else
    {
        std::cerr << "Sprite Sheet: " << m_name << ", " << name << " frame not found." << std::endl;
    }
    return q;
}

sf::Vector2i SpriteSheet::getFrameSize(sf::Uint8 index) const
{
    assert(index < m_frames.size());
    return sf::Vector2i(static_cast<int>(m_frames[index].spriteSourceSize.width),
                        static_cast<int>(m_frames[index].spriteSourceSize.height));
}

sf::Uint8 SpriteSheet::getFrameCount() const
{
    return m_frames.size();
}