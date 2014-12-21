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

//normal mapping shaders for sprites

#ifndef NORMAL_MAPPING_H_
#define NORMAL_MAPPING_H_

#include <string>

namespace Shader
{
    //SFML uses fixed function, so fuck it, let's roll with it

    /*NOTE the orthographic projection means that the eye direction
    is the same across the entire quad, effectively breaking the spec
    calculations (only fragments facing directly towards the camera
    show any highlighting) so it is fudged here by using the light
    direction twice*/

    /*ANOTHER NOTE we can't pass arrays of data to SFML shaders, so
    light arrays are fudged by packing them into matrices and extracting
    again on the GPU. Not ideal, but it works*/


    static const std::string normalVertex =
        "#version 120\n" \
        "uniform mat4 u_pointLightPositions;\n" \
        "uniform vec3 u_directionalLightDirection;\n" \

        "varying vec3 v_eyeDirection;\n" \
        "varying vec3 v_directionalLightDirection;\n" \
        "varying vec3 v_pointLightDirections[3];\n" \

        "const vec3 tangent = vec3(1.0, 0.0, 0.0);\n" \
        /*"const vec3 bitangent = vec3(0.0, 1.0, 0.0);" \*/
        "const vec3 normal = vec3(0.0, 0.0, 1.0);\n" \

        "vec3 getPosition(int index)\n" \
        "{\n" \
        "    return vec3(u_pointLightPositions[0][index], u_pointLightPositions[1][index], u_pointLightPositions[3][index]);\n" \
        "}\n" \

        "void main()\n" \
        "{\n" \
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n" \
        "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n" \
        "\n" \
        "    vec3 n = normalize(gl_NormalMatrix * normal);\n" \
        "    vec3 t = normalize(gl_NormalMatrix * tangent);\n" \
        /*cross is a bit cheaper than normalize*/
        "    vec3 b = cross(n, t);\n" \
        "\n" \
        "    vec3 viewVertex = vec3(gl_ModelViewMatrix * gl_Vertex);\n" \
        "    vec3 viewDirectionalLightDirection = vec3(gl_ModelViewMatrix * vec4(u_directionalLightDirection, 1.0));\n" \
        "\n" \
        "    v_directionalLightDirection.x = dot(viewDirectionalLightDirection, t);\n" \
        "    v_directionalLightDirection.y = dot(viewDirectionalLightDirection, b);\n" \
        "    v_directionalLightDirection.z = dot(viewDirectionalLightDirection, n);\n" \

        "    vec3 pointPositions[3] = vec3[3](getPosition(0), getPosition(1), getPosition(3));\n" \
        "    for(int i = 0; i < 3; i++)\n" \
        "    {\n" \
        "        vec3 viewPointLightDir = vec3(gl_ModelViewMatrix * vec4(pointPositions[i], 1.0)) - viewVertex;\n" \
        "\n" \
        "        v_pointLightDirections[i].x = dot(viewPointLightDir, t);\n" \
        "        v_pointLightDirections[i].y = dot(viewPointLightDir, b);\n" \
        "        v_pointLightDirections[i].z = dot(viewPointLightDir, n);\n" \
        "    }\n" \
        "\n" \
        "    v_eyeDirection.x = dot(-viewVertex, t);\n" \
        "    v_eyeDirection.y = dot(-viewVertex, b);\n" \
        "    v_eyeDirection.z = dot(-viewVertex, n);\n" \
        "}\n";

    //TODO add specular exponent as uniform
    static const std::string normalFragment =
        "#version 120\n" \
        "uniform sampler2D u_diffuseMap;\n" \
        "uniform sampler2D u_normalMap;\n" \
        "uniform vec3 u_inverseRanges;\n" \
        "uniform mat4 u_pointLightColours;\n" \
        "uniform vec3 u_directionalLightColour;\n" \
        "uniform vec3 u_ambientColour;\n" \
        "\n" \
        "varying vec3 v_directionalLightDirection;\n" \
        "varying vec3 v_pointLightDirections[3];\n" \
        "varying vec3 v_eyeDirection;\n" \
        "\n" \
        "vec4 diffuseColour;\n" \
        "\n" \
        "vec3 calcLighting(vec3 normal, vec3 lightDirection, vec3 lightColour, float falloff)\n" \
        "{\n" \
        "    float diffuseAmount = max(dot(normal, lightDirection), 0.0);\n" \
        "    vec3 mixedColour = lightColour * diffuseColour.rgb * diffuseAmount * falloff;\n" \
        /*Blinn-Phong specular calc - TODO calc specular based on some amount - probably from material settings*/
        "    vec3 eyeDirection = normalize(lightDirection);\n" \
        "    vec3 halfVec = normalize(lightDirection + eyeDirection);\n" \
        "    float specularAngle = clamp(dot(normal, halfVec), 0.0, 1.0);\n" \
        /*TODO switch const exponent for variable*/
        "    vec3 specularColour = vec3(pow(specularAngle, 68.0)) * falloff;\n" \
        "    return mixedColour + specularColour; //* u_specularAmount\n" \
        "}\n" \

        "vec3 getColour(int index)\n" \
        "{\n" \
        "    return vec3(u_pointLightColours[0][index], u_pointLightColours[1][index], u_pointLightColours[3][index]);\n" \
        "}\n" \

        "void main()\n" \
        "{\n" \
        "    diffuseColour = texture2D(u_diffuseMap, gl_TexCoord[0].xy);\n" \
        "    gl_FragColor.a = diffuseColour.a;\n" \
        "    vec3 normalVector = texture2D(u_normalMap, gl_TexCoord[0].xy).rgb * 2.0 - 1.0;\n" \
        "    vec3 ambientColour = diffuseColour.rgb * u_ambientColour;\n" \
        "    vec3 blendedColour = ambientColour;\n" \
        "    vec3 pointLightColours[3] = vec3[3](getColour(0), getColour(1), getColour(3));\n" \
        "    for(int i = 0; i < 3; i++)\n" \
        "    {\n" \
        "        vec3 pointLightDirection = v_pointLightDirections[i] * u_inverseRanges[i];\n" \
        "        float falloff = clamp(1.0 - dot(pointLightDirection, pointLightDirection), 0.0, 1.0);\n" \
        "        blendedColour += calcLighting(normalVector, normalize(v_pointLightDirections[i]), pointLightColours[i], falloff);\n" \
        "    }\n" \
        /*add directional lighting*/
        "    blendedColour += calcLighting(normalVector, normalize(v_directionalLightDirection), u_directionalLightColour, 1.0);\n" \

        "    gl_FragColor.rgb = blendedColour;" \
        "}\n";
}

#endif //NORMAL_MAPPING_H_