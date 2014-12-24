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

//drawable for animated  sprite sheets

#ifndef ANISPRITE_H_
#define ANISPRITE_H_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class AnimatedSprite final : public sf::Drawable, public sf::Transformable
{
public:
    AnimatedSprite();
    explicit AnimatedSprite(const sf::Texture& t);
    AnimatedSprite(const AnimatedSprite& copy) = default;
    ~AnimatedSprite() = default;

    void update(float dt);

    void setTexture(const sf::Texture& t);
    void setNormalMap(const sf::Texture& t);
    void setShader(sf::Shader& shader);
    void setFrameSize(const sf::Vector2i& size);
    const sf::Vector2i& getFrameSize() const;
    void setFrameCount(sf::Uint8 count);
    sf::Uint8 getFrameCount() const;
    void setFrameRate(float rate);
    float getFrameRate() const;
    void setLooped(bool looped);
    bool looped() const;
    void play(sf::Int16 start = 0, sf::Int16 end = -1);
    bool playing() const;
    void setPaused(bool paused);

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

private:

    sf::Sprite m_sprite;
    sf::Texture m_normalMap;
    sf::Shader* m_shader;
    sf::Vector2i m_frameSize;
    sf::IntRect m_subRect;
    sf::Vector2u m_textureSize;

    sf::Uint8 m_frameCount;
    sf::Uint8 m_currentFrame;
    sf::Uint8 m_firstFrame;
    sf::Uint8 m_lastFrame;

    float m_frameRate;
    float m_elapsedTime;
    bool m_loop;
    bool m_playing;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    void setFrame(sf::Uint8 frame);
};

#endif //ANISPRITE_H_