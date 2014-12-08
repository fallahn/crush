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

#include <Particles.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

ParticleSystem::ParticleSystem(Particle::Type type)
    : m_texture         (nullptr),
    m_colour            (sf::Color::White),
    m_particleSize      (10.f, 10.f),
    m_type              (type),
    m_particleLifetime  (2.f),
    m_started           (false),
    m_accumulator       (0.f),
    m_vertices          (sf::Quads),
    m_needsUpdate       (true)
{

}

//public
void ParticleSystem::setTexture(const sf::Texture& t)
{
    m_texture = const_cast<sf::Texture*>(&t);
    m_particleSize = sf::Vector2f(m_texture->getSize());
}

void ParticleSystem::setColour(const sf::Color& colour)
{
    m_colour = colour;
}

void ParticleSystem::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}

void ParticleSystem::move(const sf::Vector2f& amount)
{
    m_position += amount;
}

void ParticleSystem::setParticleLifetime(float time)
{
    m_particleLifetime = time;
}

void ParticleSystem::addAffector(Affector& a)
{
    m_affectors.push_back(a);
}

void ParticleSystem::start()
{
    m_started = true;
}

void ParticleSystem::stop()
{
    m_started = false;
}

void ParticleSystem::update(float dt)
{
    //remove dead particles
    while (!m_particles.empty() && m_particles.front().lifetime <= 0)
    {
        m_particles.pop_front();
    }

    for (auto& p : m_particles)
    {
        p.lifetime -= dt;
        for (auto& a : m_affectors)
        {
            a(p, dt);
        }
    }

    m_needsUpdate = true;

    if (m_started) emit(dt);
}

void ParticleSystem::emit(float dt)
{
    const float emitRate = 30.f;
    const float interval = 1 / emitRate;

    m_accumulator += dt;
    while (m_accumulator > interval)
    {
        m_accumulator -= interval;
        addParticle(m_position);
    }
}

Particle::Type ParticleSystem::getType() const
{
    return m_type;
}

//private
void ParticleSystem::addParticle(const sf::Vector2f& position)
{
    Particle p;
    p.position = position;
    p.colour = m_colour;
    p.lifetime = m_particleLifetime;

    //TODO add any of this system's affectors

    m_particles.push_back(p);
}

void ParticleSystem::addVertex(float x, float y, float u, float v, const sf::Color& colour) const
{
    sf::Vertex vert;
    vert.position = { x, y };
    vert.texCoords = { u, v };
    vert.color = colour;

    m_vertices.append(vert);
}

void ParticleSystem::updateVertices() const
{
    sf::Vector2f halfSize = m_particleSize / 2.f;

    m_vertices.clear();
    for (auto& p : m_particles)
    {
        auto pos = p.position;
        auto colour = p.colour;

        //make particle fade based on lifetime
        float ratio = p.lifetime / m_particleLifetime;
        colour.a = static_cast<sf::Uint8>(255.f * std::max(ratio, 0.f));

        addVertex(pos.x - halfSize.x, pos.y - halfSize.y, 0.f, 0.f, colour);
        addVertex(pos.x + halfSize.x, pos.y - halfSize.y, m_particleSize.x, 0.f, colour);
        addVertex(pos.x + halfSize.x, pos.y + halfSize.y, m_particleSize.x, m_particleSize.y, colour);
        addVertex(pos.x - halfSize.x, pos.y + halfSize.y, 0.f, m_particleSize.y, colour);
    }
}

void ParticleSystem::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    if (m_needsUpdate)
    {
        updateVertices();
        m_needsUpdate = false;
    }

    states.texture = m_texture;
    rt.draw(m_vertices, states);
}