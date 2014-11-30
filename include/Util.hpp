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

#ifndef UTIL_H_
#define UTIL_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>
#include <random>
#include <ctime>
#include <cassert>

namespace
{
    std::default_random_engine rndEngine(static_cast<unsigned long>(std::time(0)));
}

namespace Util
{
    namespace Vector
    {
        //calculates dot product of 2 vectors
        static float dot(const sf::Vector2f& lv, const sf::Vector2f& rv)
        {
            return lv.x * rv.x + lv.y * rv.y;
        }
        //Returns a given vector with its length normalized to 1
        static sf::Vector2f normalise(sf::Vector2f source)
        {
            float length = std::sqrt(dot(source, source));
            if (length != 0) source /= length;
            return source;
        }
        //returns length squared
        static float lengthSquared(const sf::Vector2f& source)
        {
            return dot(source, source);
        }
        //Returns length of a given vector
        static float length(const sf::Vector2f& source)
        {
            return std::sqrt(lengthSquared(source));
        }
    }

    namespace Position
    {
        static void centreOrigin(sf::Sprite& sprite)
        {
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
        }
        static void centreOrigin(sf::Text& text)
        {
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
        }
    }

    namespace Random
    {
        static float value(float begin, float end)
        {
            assert(begin < end);
            std::uniform_real_distribution<float> dist(begin, end);
            return dist(rndEngine);
        }

        static int value(int begin, int end)
        {
            assert(begin < end);
            std::uniform_int_distribution<int> dist(begin, end);
            return dist(rndEngine);
        }
    }
}

#endif //UTIL_H_