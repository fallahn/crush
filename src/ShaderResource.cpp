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

#include <ShaderResource.hpp>
#include <UberShader.hpp>
#include <ParticleShaders.hpp>
#include <PostShaders.hpp>

namespace
{
    //TODO rename this somewhat, as it's getting a tad verbose
    namespace Defines
    {
        static const std::string version = "#version 120\n";
        static const std::string diffuseMap = "#define DIFFUSE_MAP\n";
        static const std::string normalMap = "#define BUMP_MAP\n";
        static const std::string vertColour = "#define VERTEX_COLOUR\n";
        static const std::string vertMultiply = "#define VERTEX_MULTIPLY\n";
        static const std::string specular = "#define SPECULAR\n";
        static const std::string reflection = "#define REFLECT_MAP\n";
        static const std::string environment = "#define SKY_MAP\n";
    }
}

sf::Shader& ShaderResource::get(Shader::Type type)
{
    auto result = m_shaders.find(type);
    if (result != m_shaders.end())
    {
        return *result->second;
    }

    //NOTE shader not working properly when using scene lighting? MAKE SURE IT HAS BEEN ADDED TO SCENE SHADERS
    Shader::Ptr shader = std::make_unique<sf::Shader>();
    switch (type)
    {
    case Shader::Type::FlatShaded:
        shader->loadFromMemory(Defines::version + Defines::vertColour + Shader::uberVertex,
            Defines::version + Defines::diffuseMap + Defines::specular + Defines::vertMultiply + Shader::uberFragment);
        break;
    case Shader::Type::NormalMap:
        shader->loadFromMemory(Defines::version + Shader::uberVertex,
            Defines::version + Defines::diffuseMap + Defines::normalMap + Shader::uberFragment);
        break;
    case Shader::Type::NormalMapSpecular:        
        shader->loadFromMemory(Defines::version + Defines::vertColour + Shader::uberVertex,
            Defines::version + Defines::diffuseMap + Defines::normalMap + Defines::specular + Defines::vertMultiply + Shader::uberFragment);
        break;
    case Shader::Type::Water:
        shader->loadFromMemory(Defines::version + Defines::vertColour + Defines::environment + Shader::uberVertex,
            Defines::version + Defines::specular + Defines::normalMap + Defines::environment + Shader::uberFragment);
        break;
    case Shader::Type::WaterDrop:
        shader->loadFromMemory(Defines::version + Defines::vertColour + Defines::environment + Shader::uberVertex,
            Defines::version + Defines::diffuseMap + Defines::normalMap + Defines::environment + Defines::specular + Defines::vertMultiply + Shader::uberFragment);
        break;
    case Shader::Type::Metal:
        shader->loadFromMemory(Defines::version + Defines::vertColour + Defines::reflection + Shader::uberVertex,
            Defines::version + Defines::diffuseMap + Defines::normalMap + Defines::specular + Defines::reflection + Shader::uberFragment);
        break;
    case Shader::Type::GaussianBlur:
        shader->loadFromMemory(Defines::version + Shader::gaussianFrag, sf::Shader::Fragment);
        break;
    default: break;
    }

    m_shaders.insert(std::make_pair(type, std::move(shader)));
    return *m_shaders[type];
}

void ShaderResource::addBinding(Shader::UniformBinding::Ptr& b)
{
    m_uniformBindings.push_back(std::move(b));
}

void ShaderResource::updateBindings()
{
    for (auto& b : m_uniformBindings)
        b->bind();
}