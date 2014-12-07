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

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <array>
#include <functional>

class Particle final : private sf::NonCopyable
{
    friend class ParticleSystem;
public:
    typedef std::array<sf::Vertex*, 4u> Quad;
    typedef std::unique_ptr<Particle> Ptr;
    typedef std::function<void(Particle& p, float dt)> Affector;

    enum class State
    {
        Alive,
        Dying,
        Dead
    };

    explicit Particle(Quad& q);
    ~Particle() = default;

    void update(float dt);
    void spawn(const sf::Vector2f& position, const sf::Vector2f& velocity, float force, float rotation);
    State getState() const;
    void kill();
    void setTexture(const sf::Texture& t);

protected:
    void setForce(float force);
    float getForce() const;
    void setVelocity(const sf::Vector2f& velocity);
    const sf::Vector2f& getVelocity() const;
    void setState(State state);

    void setColour(const sf::Color& c);
    void move(const sf::Vector2f& amount);
    void setPosition(const sf::Vector2f& position);
    void rotate(float amount);
    void setRotation(float rotation);
    void setSize(const sf::Vector2f& size);
    void scale(const sf::Vector2f& amount);
    void setScale(const sf::Vector2f& scale);

    void applyTransform();
private:
    float m_force;
    sf::Vector2f m_velocity;
    State m_state;

    Quad m_vertices;
    sf::Vector2f m_position;
    sf::Vector2f m_size, m_scale;
    float m_rotation;
    bool m_applyTransform;
};

class ParticleSystem final : public sf::Drawable, private sf::Transformable, private sf::NonCopyable
{
public:
    enum class Type
    {
        Splat
    };

    typedef std::unique_ptr<ParticleSystem> Ptr;
    ParticleSystem(float emissionRate, sf::Uint16 count, std::function<Particle::Ptr(Particle::Quad& verts)>& create);
    ~ParticleSystem() = default;

    void update(float dt);
    void start(float duration = 0.f);
    void stop();

    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);
    void setRandomVelocity(bool rndVel);
    void setStrength(float strength);
    void setRotation(float rotation);

    void kill();
    void setTexture(const sf::Texture& t);
    void setBlendMode(sf::BlendMode blendmode);

private:

    sf::VertexArray m_vertices;
    std::vector<Particle::Ptr> m_particles;
    sf::Uint16 m_particleCount, m_currentIndex;
    float m_emissionRate;
    sf::Clock m_emitClock, m_durationClock;
    sf::Vector2f m_velocity;
    float m_force, m_rotation, m_duration;
    bool m_enabled, m_randomVelocity;
    const sf::Texture* m_texture;
    sf::BlendMode m_blendMode;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //PARTICLES_H_
