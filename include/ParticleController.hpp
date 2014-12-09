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

//in charge of particle systems used throughout the world

#ifndef PARTICLE_CONTROLLER_H_
#define PARTICLE_CONTROLLER_H_

#include <Observer.hpp>
#include <Particles.hpp>
#include <Resource.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>

class ParticleController final : public Observer, private sf::NonCopyable, public sf::Drawable
{
public:
    explicit ParticleController(TextureResource& tr);
    ~ParticleController() = default;

    void update(float dt);

    void onNotify(Subject& s, const game::Event& evt) override;

private:
    std::vector<ParticleSystem> m_systems;
    TextureResource& m_textureResource;

    ParticleSystem& addSystem(Particle::Type type);
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif //PARTICLE_CONTROLLER_H_