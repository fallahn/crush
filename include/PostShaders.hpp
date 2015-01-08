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

//shaders used in post process effects

#ifndef POST_FX_H_
#define POST_FX_H_

#include <string>

namespace Shader
{
    static const std::string gaussianFrag =
        "uniform sampler2D u_diffuse;\n" \
        "uniform vec2 u_offset;\n" \
        "void main()\n" \
        "{\n" \
            "vec2 coords = gl_TexCoord[0].xy;\n" \
            "vec4 colour = vec4(0.0);\n" \
            "colour += texture2D(u_diffuse, coords - 4.0 * u_offset) * 0.0162162162;\n" \
            "colour += texture2D(u_diffuse, coords - 3.0 * u_offset) * 0.0540540541;\n" \
            "colour += texture2D(u_diffuse, coords - 2.0 * u_offset) * 0.1216216216;\n" \
            "colour += texture2D(u_diffuse, coords - u_offset) * 0.1945945946;\n" \
            "colour += texture2D(u_diffuse, coords) * 0.2270270270;\n" \
            "colour += texture2D(u_diffuse, coords + u_offset) * 0.1945945946;\n" \
            "colour += texture2D(u_diffuse, coords + 2.0 * u_offset) * 0.1216216216;\n" \
            "colour += texture2D(u_diffuse, coords + 3.0 * u_offset) * 0.0540540541;\n" \
            "colour += texture2D(u_diffuse, coords + 4.0 * u_offset) * 0.0162162162;\n" \
            "gl_FragColor = colour;\n" \
        "}\n";
}
#endif //POST_FX_H_