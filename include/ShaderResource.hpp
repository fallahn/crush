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

//shaders are handle a little differently to other resources, so have their own manager
#ifndef SHADER_RESOURCE_H_
#define SHADER_RESOURCE_H_

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <memory>
#include <map>

namespace Shader
{
    enum class Type
    {
        FlatShaded,
        NormalMap,
        NormalMapSpecular,
        Water
    };

    typedef std::unique_ptr<sf::Shader> Ptr;
}

class ShaderResource final : private sf::NonCopyable
{
public:

    ShaderResource() = default;
    ~ShaderResource() = default;

    sf::Shader& get(Shader::Type type);

private:
    std::map<Shader::Type, Shader::Ptr> m_shaders;
};

#endif //SHADER_RESOURCE_H_