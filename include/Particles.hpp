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

#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <Affectors.hpp>

#include <deque>
#include <functional>
#include <vector>

struct Particle final : public sf::Transformable
{    
    enum class Type
    {
        Splat,
        Splash
    };

    //sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color colour;
    float lifetime = 0.f;
};

class ParticleSystem final : public sf::Drawable
{
public:
    typedef std::function<void(Particle& p, float dt)> Affector;

    explicit ParticleSystem(Particle::Type type);
    ~ParticleSystem() = default;

    void setTexture(const sf::Texture& t);
    void setColour(const sf::Color& colour);
    void setBlendMode(sf::BlendMode mode);
    void setShader(sf::Shader& shader);

    void setPosition(const sf::Vector2f& position);
    void move(const sf::Vector2f& amount);

    void setParticleLifetime(float time);
    void setInitialVelocity(const sf::Vector2f& vel);
    void setRandomInitialVelocity(bool b);

    void addAffector(Affector& a);
    template <typename T>
    void addAffector(T& affector);

    void start(sf::Uint8 releaseCount = 1, float duration = 0.f);
    bool started() const;
    void stop();
    void update(float dt);
    
    Particle::Type getType() const;
    sf::Uint32 getParticleCount() const;


private:
    std::deque<Particle> m_particles;
    sf::Texture* m_texture;
    sf::Color m_colour;
    sf::Vector2f m_position;
    sf::Vector2f m_particleSize;
    sf::Vector2f m_texCoords;
    float m_particleLifetime;
    Particle::Type m_type;

    sf::Vector2f m_initialVelocity;
    bool m_randVelocity;

    bool m_started;
    float m_accumulator;
    
    std::vector<Affector> m_affectors;

    mutable sf::VertexArray m_vertices;
    mutable bool m_needsUpdate;

    sf::Clock m_durationClock;
    float m_duration;
    sf::Uint8 m_releaseCount;

    sf::BlendMode m_blendMode;
    sf::Shader* m_shader;

    void emit(float dt);
    void addParticle(const sf::Vector2f& position);
    void addVertex(const sf::Vector2f& position, float u, float v, const sf::Color& colour)const;
    void updateVertices() const;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

template <typename T>
void ParticleSystem::addAffector(T& affector)
{
    Affector a = std::bind(affector, std::placeholders::_1, std::placeholders::_2);
    m_affectors.push_back(a);
}

#endif //PARTICLES_H_
