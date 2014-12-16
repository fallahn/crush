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

#include <ParticleController.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

ParticleController::ParticleController(TextureResource& tr)
    : m_textureResource (tr)
{
    m_systems.reserve(50);
}

//public
void ParticleController::update(float dt)
{
    for (auto& p : m_systems)
        p.update(dt);
}

void ParticleController::onNotify(Subject& s, const game::Event& evt)
{
    if (evt.type == game::Event::Node
        && evt.node.action == game::Event::NodeEvent::Despawn)
    {
        //raise a 'splat' effect
        sf::Vector2f evtPosition(evt.node.positionX, evt.node.positionY);
        sf::Color colour;
        switch (evt.node.type)
        {
        case Category::PlayerOne:
        case Category::PlayerTwo:
            colour = sf::Color::Blue;
            break;
        case Category::Npc:
            colour = sf::Color::Green;
            break;
        default: break;
        }

        //find and idle system and use it
        auto result = std::find_if(m_systems.begin(), m_systems.end(),
            [](const ParticleSystem& ps)
        {
            return (ps.getType() == Particle::Type::Splat && !ps.started());
        });

        if (result != m_systems.end())
        {
            result->setColour(colour);
            result->setPosition(evtPosition);
            result->start(6u, 0.1f);
        }

        //else append a new system
        else
        {
            auto& particleSystem = addSystem(Particle::Type::Splat);
            particleSystem.setColour(colour);
            particleSystem.setPosition(evtPosition);
            particleSystem.start(6u, 0.1f);
        } 
    }
}

//private
ParticleSystem& ParticleController::addSystem(Particle::Type type)
{
    //TODO other system types
    m_systems.emplace_back(type);
    ParticleSystem& particleSystem = m_systems.back();
    if (type == Particle::Type::Splat)
    {
        particleSystem.setRandomInitialVelocity(true);
        ForceAffector fa({ 0.f, 4000.f });
        particleSystem.addAffector(fa);
        RotateAffector ra(380.f);
        particleSystem.addAffector(ra);
        ScaleAffector sa({ 5.5f, 5.5f });
        particleSystem.addAffector(sa);
        particleSystem.setTexture(m_textureResource.get("res/textures/particle.png"));
    }
    else if (type == Particle::Type::Splash)
    {
        particleSystem.setRandomInitialVelocity(true);
        ForceAffector fa({ 0.f, 4000.f });
        particleSystem.addAffector(fa);
        RotateAffector ra(380.f);
        particleSystem.addAffector(ra);
        ScaleAffector sa({ 5.5f, 5.5f });
        particleSystem.addAffector(sa);
        particleSystem.setTexture(m_textureResource.get("res/textures/water_splash.png"));
        //TODO set blendmode to add
        //TODO use shader to discard alpha to crop falloff
    }
    return particleSystem;
}

void ParticleController::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    //only draw visible.
    for (auto& p : m_systems) 
        if(p.getParticleCount() > 0) 
            rt.draw(p, states);
}