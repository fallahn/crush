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

//drawable for splishy splashy water

#ifndef WATER_DRAWABLE_H_
#define WATER_DRAWABLE_H_

#include <Resource.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <vector>

class WaterDrawable final : public sf::Drawable, private sf::NonCopyable
{
public:
    explicit WaterDrawable(TextureResource& tr, sf::Shader& shader, const sf::Vector2f& size = sf::Vector2f(20.f, 20.f));
    ~WaterDrawable() = default;

    void splash(float position, float speed);
    void update(float dt);

    void setSize(const sf::Vector2f& size);
    void setColours(const sf::Color& lightColour, const sf::Color& darkColour);

private:
    struct Column
    {
        Column();
        void update(float dt);

        float targetHeight;
        float height;
        float speed;
    };

    sf::Vector2f m_size;
    sf::Color m_lightColour;
    sf::Color m_darkColour;

    std::vector<Column> m_columns;
    mutable sf::VertexArray m_vertices;

    sf::Texture m_normalTexture;
    float m_texHeight;
    sf::Shader* m_shader;

    sf::Uint8 m_waveIndex;
    float m_waveTime;

    void resize();
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};



#endif //WATER_DRAWABLE_H_