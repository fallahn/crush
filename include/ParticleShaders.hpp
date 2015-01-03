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

//shaders used in particle effects

#ifndef PARTICLE_SHADERS_H_
#define PARTICLE_SHADERS_H_

#include <string>

namespace Shader
{
    static const std::string waterGlob =
        "#version 120\n" \
        "const float alphaCutoff = 0.4;\n" \
        "const float finalAlpha = 0.55;\n" \
        "uniform sampler2D u_diffuseMap;\n" \
        "void main()\n" \
        "{\n" \
        "   vec4 textureColour = texture2D(u_diffuseMap, gl_TexCoord[0].xy);\n" \
        "   if(textureColour.a < alphaCutoff) discard;\n" \
        "   textureColour *= gl_Color;\n" \
        "   textureColour.a *= finalAlpha;\n" \
        "   gl_FragColor = textureColour;\n" \
        "}";
}

#endif // PARTICLE_SHADERS_H_