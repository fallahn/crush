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

#include <AnimatedSprite.hpp>
#include <Resource.hpp>
#include <Util.hpp>
#include <JsonUtil.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <picojson.h>

#include <cassert>
#include <fstream>

AnimatedSprite::AnimatedSprite()
    : m_shader      (nullptr),
    m_frameCount    (0u),
    m_currentFrame  (0u),
    m_firstFrame    (0u),
    m_lastFrame     (0u),
    m_frameRate     (0.f),
    m_elapsedTime   (0.f),
    m_loop          (false),
    m_playing       (false){}

AnimatedSprite::AnimatedSprite(const sf::Texture& t)
    : m_sprite      (t),
    m_shader        (nullptr),
    m_textureSize   (t.getSize()),
    m_frameCount    (0u),
    m_currentFrame  (0u),
    m_firstFrame    (0u),
    m_lastFrame     (0u),
    m_frameRate     (0.f),
    m_elapsedTime   (0.f),
    m_loop          (false),
    m_playing       (false){}

AnimatedSprite::AnimatedSprite(const std::string& propertiesPath, TextureResource& tr)
    : m_shader      (nullptr),
    m_frameCount    (0u),
    m_currentFrame  (0u),
    m_firstFrame    (0u),
    m_lastFrame     (0u),
    m_frameRate     (0.f),
    m_elapsedTime   (0.f),
    m_loop          (false),
    m_playing       (false)
{
    std::ifstream file(propertiesPath);
    assert(file.good());
    assert(Util::File::validLength(file));

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
        //get array of animations
        if (pv.get("Animations").is<picojson::array>())
        {
            const auto& anims = pv.get("Animations").get<picojson::array>();
            for (const auto& a : anims)
            {
                std::string name = (a.get("Name").is<std::string>()) ? a.get("Name").get<std::string>() : "";
                sf::Int16 start = (a.get("Start").is<double>()) ? static_cast<sf::Int16>(a.get("Start").get<double>()) : 0;
                sf::Int16 end = (a.get("End").is<double>()) ? static_cast<sf::Int16>(a.get("End").get<double>()) : 0;
                bool loop = (a.get("Loop").is<bool>()) ? a.get("Loop").get<bool>() : false;
                m_animations.emplace_back(name, start, end, loop);
            }
        }

        //properties
        if (pv.get("FrameCount").is<double>())
            m_frameCount = static_cast<sf::Uint8>(pv.get("FrameCount").get<double>());
        else
            std::cerr << propertiesPath << " missing frame count" << std::endl;

        if (pv.get("FrameSize").is<std::string>())
            setFrameSize(Util::Vector::vec2FromString<int>(pv.get("FrameSize").get<std::string>()));
        else
            std::cerr << propertiesPath << " missing frame size" << std::endl;

        if (pv.get("FrameRate").is<double>())
            m_frameRate = static_cast<float>(pv.get("FrameRate").get<double>());
        else
            std::cerr << propertiesPath << " missing frame rate" << std::endl;

        std::string filePath;
        auto result = propertiesPath.find_last_of('/');
        if (result != std::string::npos)
            filePath = propertiesPath.substr(0, result + 1);

        if (pv.get("Texture").is<std::string>())
            setTexture(tr.get(filePath + pv.get("Texture").get<std::string>()));
        else
            std::cerr << propertiesPath << " missing texture name" << std::endl;

        if (pv.get("NormalMap").is<std::string>())
            setNormalMap(tr.get(filePath + pv.get("NormalMap").get<std::string>()));
    }
    else
    {
        std::cerr << "Animated Sprite: " << err << std::endl;
    }

}

//public
void AnimatedSprite::update(float dt)
{
    if (m_playing)
    {
        float frameTime = 1.f / m_frameRate;
        m_elapsedTime += dt;

        while (m_elapsedTime >= frameTime && (m_currentFrame < m_lastFrame || m_loop))
        {
            //while not at the end of the range or loop set to true
            //move frame to the right
            m_subRect.left += m_frameSize.x;

            //wrap around if at texture edge
            if (m_subRect.left + m_subRect.width > m_textureSize.x)
            {
                m_subRect.left = 0;
                m_subRect.top += m_frameSize.y;
            }

            //update frame data
            m_elapsedTime -= frameTime;
            
            m_currentFrame++;
            if (m_currentFrame > m_lastFrame)
            {
                if (m_loop)
                    setFrame(m_firstFrame);
                else
                    m_playing = false;
            }
        }

        m_sprite.setTextureRect(m_subRect);
    }
}

void AnimatedSprite::setTexture(const sf::Texture& t)
{
    m_sprite.setTexture(t);
    m_textureSize = t.getSize();
}

const sf::Texture* AnimatedSprite::getTexture() const
{
    return m_sprite.getTexture();
}

void AnimatedSprite::setNormalMap(const sf::Texture& t)
{
    m_normalMap = t;
}

void AnimatedSprite::setShader(sf::Shader& shader)
{
    m_shader = &shader;
}

void AnimatedSprite::setFrameSize(const sf::Vector2i& size)
{
    m_frameSize = size;
    m_subRect.width = size.x;
    m_subRect.height = size.y;
}

const sf::Vector2i& AnimatedSprite::getFrameSize() const
{
    return m_frameSize;
}

void AnimatedSprite::setFrameCount(sf::Uint8 count)
{
    m_frameCount = count;
}

void AnimatedSprite::setFrameRate(float rate)
{
    assert(rate >= 0.f);
    m_frameRate = rate;
    //m_playing = (rate > 0.f);
}

float AnimatedSprite::getFrameRate() const
{
    return m_frameRate;
}

void AnimatedSprite::setLooped(bool loop)
{
    m_loop = loop;
}

bool AnimatedSprite::looped() const
{
    return m_loop;
}

void AnimatedSprite::play(sf::Int16 start, sf::Int16 end)
{
    assert(start < m_frameCount && (end < m_frameCount));

    m_playing = !(start == end);// ? false : true;
    m_currentFrame = m_firstFrame = start;
    m_lastFrame = (end < 0) ? m_frameCount - 1 : end;

    //set position of starting frame / sub rect
    setFrame(start);
}

void AnimatedSprite::play(Animation animation)
{
    setLooped(animation.m_loop);
    play(animation.m_startFrame, animation.m_endFrame);
}

bool AnimatedSprite::playing() const
{
    return m_playing;
}

void AnimatedSprite::setPaused(bool paused)
{
    m_playing = !paused;
}

void AnimatedSprite::setColour(const sf::Color& c)
{
    m_sprite.setColor(c);
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


//private
void AnimatedSprite::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{    
    if (m_shader)
    {
        m_shader->setParameter("u_normalMap", m_normalMap);
        m_shader->setParameter("u_diffuseMap", *m_sprite.getTexture());
    }
    states.transform *= getTransform();
    states.shader = m_shader;
    rt.draw(m_sprite, states);
}

void AnimatedSprite::setFrame(sf::Uint8 index)
{
    assert(index < m_frameCount);

    auto position = m_textureSize.x / m_frameSize.x;

    assert(position > 0);

    auto x = index % position;
    auto y = index / position;

    m_subRect.left = x * m_frameSize.x;
    m_subRect.top = y * m_frameSize.y;

    m_sprite.setTextureRect(m_subRect);
    m_currentFrame = index;
}