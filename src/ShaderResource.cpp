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

#include <ShaderResource.hpp>
#include <NormalMapping.hpp>
#include <ParticleShaders.hpp>

namespace Shader
{
    namespace Defines
    {
        static const std::string version = "#version 120\n";
        static const std::string diffuseMap = version + "#define DIFFUSE_MAP\n";
        static const std::string vertColour = version + "#define VERTEX_COLOUR\n";
    }
}

sf::Shader& ShaderResource::get(Shader::Type type)
{
    auto result = m_shaders.find(type);
    if (result != m_shaders.end())
    {
        return *result->second;
    }

    Shader::Ptr shader = std::make_unique<sf::Shader>();
    switch (type)
    {
    case Shader::Type::NormalMap:        
        shader->loadFromMemory(Shader::Defines::version + Shader::normalVertex, Shader::Defines::diffuseMap + Shader::normalFragment);      
        break;
    case Shader::Type::Water:
        shader->loadFromMemory(Shader::Defines::vertColour + Shader::normalVertex, Shader::Defines::version + Shader::normalFragment);
        break;
    default: break;
    }

    m_shaders.insert(std::make_pair(type, std::move(shader)));
    return *m_shaders[type];
}