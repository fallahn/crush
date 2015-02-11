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

//multi purpose shader for game sprite

#ifndef UBER_SHADER_H_
#define UBER_SHADER_H_

#include <string>

namespace Shader
{
    /*NOTE the orthographic projection means that the eye direction
    is the same across the entire quad, effectively breaking the spec
    calculations (only fragments facing directly towards the camera
    show any highlighting) so it is fudged here by using the light
    direction twice*/

    /*ANOTHER NOTE we can't pass arrays of data to SFML shaders, so
    light arrays are fudged by packing them into matrices and extracting
    again on the GPU. Not ideal, but it works*/

    /*SKY_MAP is the scene reflected vertically for effects like water
    REFLECT_MAP is the scene reflected horizontally for metal type reflection*/

    static const std::string uberVertex =
        "#define LIGHT_COUNT 6\n" \
        "uniform mat4 u_pointLightPositionsFirst;\n" \
        "uniform mat4 u_pointLightPositionsSecond;\n" \
        "uniform vec3 u_directionalLightDirection;\n" \
        "uniform mat4 u_inverseWorldViewMatrix;\n" \

        "varying vec3 v_eyeDirection;\n" \
        "varying vec3 v_directionalLightDirection;\n" \
        "varying vec3 v_pointLightDirections[LIGHT_COUNT];\n" \

        "const vec3 tangent = vec3(1.0, 0.0, 0.0);\n" \
        /*"const vec3 bitangent = vec3(0.0, 1.0, 0.0);" \*/
        "const vec3 normal = vec3(0.0, 0.0, 1.0);\n" \

        "const vec2 sceneSize = vec2(1920.0, 1080.0);\n" \


        "vec3[LIGHT_COUNT] unpackLightPositions()\n" \
        "{\n" \
        "return vec3[LIGHT_COUNT](\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsFirst[0][0], u_pointLightPositionsFirst[1][0], u_pointLightPositionsFirst[3][0], 1.0)),\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsFirst[0][1], u_pointLightPositionsFirst[1][1], u_pointLightPositionsFirst[3][1], 1.0)),\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsFirst[0][3], u_pointLightPositionsFirst[1][3], u_pointLightPositionsFirst[3][3], 1.0)),\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsSecond[0][0], u_pointLightPositionsSecond[1][0], u_pointLightPositionsSecond[3][0], 1.0)),\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsSecond[0][1], u_pointLightPositionsSecond[1][1], u_pointLightPositionsSecond[3][1], 1.0)),\n" \
        "vec3(u_inverseWorldViewMatrix * vec4(u_pointLightPositionsSecond[0][3], u_pointLightPositionsSecond[1][3], u_pointLightPositionsSecond[3][3], 1.0)));\n" \
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
        "#if defined(SKY_MAP)\n" \
        "    gl_TexCoord[1].xy = vec2(viewVertex.x, (viewVertex.y - (2.0 * gl_Vertex.y))) / sceneSize;\n" \
        "#endif\n" \

        "#if defined(REFLECT_MAP)\n" \
        "    gl_TexCoord[2].xy = viewVertex.xy / sceneSize;\n" \
        "    gl_TexCoord[2].x = 1.0 - gl_TexCoord[2].x;\n" \
        "#endif\n" \

        
        "    vec3 viewDirectionalLightDirection = vec3(gl_ModelViewMatrix * u_inverseWorldViewMatrix * vec4(u_directionalLightDirection, 1.0));\n" \
        "\n" \
        "    v_directionalLightDirection.x = dot(viewDirectionalLightDirection, t);\n" \
        "    v_directionalLightDirection.y = dot(viewDirectionalLightDirection, b);\n" \
        "    v_directionalLightDirection.z = dot(viewDirectionalLightDirection, n);\n" \

        "    vec3 pointPositions[LIGHT_COUNT] = unpackLightPositions();\n" \
        "    for(int i = 0; i < LIGHT_COUNT; i++)\n" \
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

        "#if defined(VERTEX_COLOUR)\n" \
        "    gl_FrontColor = gl_Color;\n" \
        "#endif\n" \
        "}\n";

    //TODO add specular exponent as uniform
    static const std::string uberFragment =
        "#define LIGHT_COUNT 6\n" \
        "uniform sampler2D u_diffuseMap;\n" \
        "uniform sampler2D u_normalMap;\n" \
        "#if defined(REFLECT_MAP) || defined(SKY_MAP)\n" \
        "uniform sampler2D u_reflectMap;\n" \
        "#define SPEC_AMOUNT 1.0\n" \
        "#else\n" \
        "#define SPEC_AMOUNT 0.5\n" \
        "#endif\n" \
        "uniform vec3 u_inverseRangesFirst;\n" \
        "uniform vec3 u_inverseRangesSecond;\n" \
        "uniform mat4 u_pointLightColoursFirst;\n" \
        "uniform mat4 u_pointLightColoursSecond;\n" \
        "uniform vec3 u_directionalLightColour;\n" \
        "uniform vec3 u_ambientColour;\n" \
        "uniform float u_textureOffset;\n" \
        "\n" \
        "varying vec3 v_directionalLightDirection;\n" \
        "varying vec3 v_pointLightDirections[LIGHT_COUNT];\n" \
        "varying vec3 v_eyeDirection;\n" \
        "\n" \
        "vec4 diffuseColour;\n" \
        "vec4 normalColour = vec4(0.5, 0.5, 1.0, 0.0);\n" \
        "float[LIGHT_COUNT] inverseRanges = float[LIGHT_COUNT](u_inverseRangesFirst.x, u_inverseRangesFirst.y, u_inverseRangesFirst.z, u_inverseRangesSecond.x, u_inverseRangesSecond.y, u_inverseRangesSecond.z);\n" \
        "\n" \
        "vec3 calcLighting(vec3 normal, vec3 lightDirection, vec3 lightColour, float falloff)\n" \
        "{\n" \
        "    float diffuseAmount = max(dot(normal, lightDirection), 0.0);\n" \
        "    vec3 mixedColour = lightColour * diffuseColour.rgb * diffuseAmount * falloff;\n" \

        /*Blinn-Phong specular calc - TODO calc specular based on some amount - probably from material settings*/
        "#if defined(SPECULAR)\n" \
        "    vec3 eyeDirection = normalize(lightDirection);\n" \
        "    vec3 halfVec = normalize(lightDirection + eyeDirection);\n" \
        "    float specularAngle = clamp(dot(normal, halfVec), 0.0, 1.0);\n" \
        /*TODO switch const exponent for variable*/
        "    vec3 specularColour = vec3(pow(specularAngle, 96.0)) * falloff;\n" \

        "    return mixedColour + (specularColour * SPEC_AMOUNT);//normalColour.a);\n" \
        "#else\n" \
        "    return mixedColour;\n" \
        "#endif\n" \
        "}\n" \


        "vec3[LIGHT_COUNT] unpackLightColours()\n" \
        "{\n" \
            "return vec3[LIGHT_COUNT](\n" \
                "vec3(u_pointLightColoursFirst[0][0], u_pointLightColoursFirst[1][0], u_pointLightColoursFirst[3][0]),\n" \
                "vec3(u_pointLightColoursFirst[0][1], u_pointLightColoursFirst[1][1], u_pointLightColoursFirst[3][1]),\n" \
                "vec3(u_pointLightColoursFirst[0][3], u_pointLightColoursFirst[1][3], u_pointLightColoursFirst[3][3]),\n" \
                "vec3(u_pointLightColoursSecond[0][0], u_pointLightColoursSecond[1][0], u_pointLightColoursSecond[3][0]),\n" \
                "vec3(u_pointLightColoursSecond[0][1], u_pointLightColoursSecond[1][1], u_pointLightColoursSecond[3][1]),\n" \
                "vec3(u_pointLightColoursSecond[0][3], u_pointLightColoursSecond[1][3], u_pointLightColoursSecond[3][3]));\n" \
        "}\n" \


        "void main()\n" \
        "{\n" \
        "#if defined(BUMP_MAP)\n" \
        "    normalColour = texture2D(u_normalMap, (gl_TexCoord[0].xy + u_textureOffset));\n" \
        "#endif\n" \

        "#if defined(DIFFUSE_MAP)\n" \
        "    diffuseColour = texture2D(u_diffuseMap, gl_TexCoord[0].xy);\n" \
        "#else\n" \
        "    diffuseColour = gl_Color;\n" \
        "#endif\n" \

        "#if defined(SKY_MAP)\n" \
        "    vec2 coordS = vec2(gl_TexCoord[1].xy - (normalColour.rg * 0.01));\n" \
        "    coordS.y += sin((coordS.x * 300.0) + (u_textureOffset * 120.0)) * 0.002;\n" \
        "    diffuseColour.rgb *= texture2D(u_reflectMap, coordS).rgb;\n" \

        "#endif\n" \

        "#if defined(REFLECT_MAP)\n" \
        "    vec2 coordR = vec2(gl_TexCoord[2].xy- (normalColour.rg * 0.5));\n" \
        "    diffuseColour.rgb = mix(texture2D(u_reflectMap, coordR).rgb, diffuseColour.rgb, 0.4);\n" \
        "#endif\n" \

        "    gl_FragColor.a = diffuseColour.a;\n" \
        "    vec3 normalVector = normalColour.rgb * 2.0 - 1.0;\n" \
        "    vec3 ambientColour = diffuseColour.rgb * u_ambientColour;\n" \
        "    vec3 blendedColour = ambientColour;\n" \
        "    vec3 pointLightColours[LIGHT_COUNT] = unpackLightColours();\n" \
        
        "    for(int i = 0; i < LIGHT_COUNT; i++)\n" \
        "    {\n" \
        "        vec3 pointLightDirection = v_pointLightDirections[i] * inverseRanges[i];\n" \
        "        float falloff = clamp(1.0 - dot(pointLightDirection, pointLightDirection), 0.0, 1.0);\n" \
        "        blendedColour += calcLighting(normalVector, normalize(v_pointLightDirections[i]), pointLightColours[i], falloff);\n" \
        "    }\n" \
        /*add directional lighting*/
        "    blendedColour += calcLighting(normalVector, normalize(v_directionalLightDirection), u_directionalLightColour, 1.0);\n" \

        "    gl_FragColor.rgb = blendedColour;\n" \
        "#if defined(VERTEX_MULTIPLY)\n" \
        "    gl_FragColor *= gl_Color;\n" \
        "#endif\n" \
        "}\n";
}

#endif //UBER_SHADER_H_