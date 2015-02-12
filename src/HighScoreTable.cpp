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

#include <HighScoreTable.hpp>

#include <fstream>
#include <algorithm>
#include <iostream>

namespace
{
    const sf::Uint8 maxEntries = 50u;
    const std::string filename = "scores.crs";
}

std::vector<HighScoreTable::Entry> HighScoreTable::read()
{
    std::vector<HighScoreTable::Entry> results;

    std::ifstream file(filename);
    if (file.good())
    {
        while (!file.eof())
        {
            std::string line;
            file >> line;
            auto pos = line.find_last_of(',');
            if (pos != std::string::npos && pos != line.size())
            {
                try
                {
                    std::string name = line.substr(0, pos);
                    sf::Int32 score = std::atoi(line.substr(pos + 1).c_str());
                    results.emplace_back(std::make_pair(name, score));
                }
                catch (...)
                {/*failed to parse score for some reason*/}
            }
        }
        file.close();
    }

    if (results.size() > 0)
    {
        std::sort(results.begin(), results.end(), Sort());
        while (results.size() > maxEntries) results.pop_back();
    }

    return results;
}

void HighScoreTable::write(std::vector<HighScoreTable::Entry>& data)
{
    std::ofstream file(filename, std::ios::out);
    if (file.good())
    {
        std::sort(data.begin(), data.end(), Sort());
        while (data.size() > maxEntries) data.pop_back();

        for (const auto& e : data)
        {
            file << e.first + "," + std::to_string(e.second) << std::endl;
        }
        file.close();
    }
    else
    {
        std::cerr << "failed to open high score file for writing" << std::endl;
    }
}