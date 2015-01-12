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

//controls spawning of solid / block / collectable items in world

#ifndef MAP_CONTROLLER_H_
#define MAP_CONTROLLER_H_

#include <CommandStack.hpp>
#include <Resource.hpp>
#include <ShaderResource.hpp>
#include <Observer.hpp>
#include <Map.hpp>
#include <AnimatedSprite.hpp>
#include <WaterDrawable.hpp>
#include <SpriteSheet.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <functional>
#include <list>
#include <memory>

class Map;
class MapController final : private sf::NonCopyable, public Observer
{
public:
    enum class MapDrawable
    {
        Item,
        Solid,
        Block,
        Water,
        RearDetail,
        FrontDetail,
        Background,
        Hat
    };

    MapController(CommandStack& cs, TextureResource& tr, ShaderResource& sr);
    ~MapController() = default;

    void update(float dt);

    void setSpawnFunction(std::function<void(const Map::Node&)>& func);
    void loadMap(const Map& map);

    sf::Drawable* getDrawable(MapDrawable type);

    void onNotify(Subject& s, const Event& e) override;

private:
    struct Item
    {
        Item(const Map::Node& pos, float lt)
            : node(pos), lifeTime(lt){}
        Map::Node node;
        float lifeTime;
    };

    CommandStack& m_commandStack;

    std::vector<Item> m_items;
    float m_itemTime;
    bool m_itemActive;

    TextureResource& m_textureResource;
    ShaderResource& m_shaderResource;
    AnimatedSprite m_itemSprite;
    AnimatedSprite m_blockSprite;
    AnimatedSprite m_backgroundSprite;
    AnimatedSprite m_hatSprite;

    std::list<WaterDrawable> m_waterDrawables;


    std::function<void(const Map::Node&)> spawn;
    void shuffleItems();
    void spawnHat();

    //used as shader binding
    const sf::Texture& getBackgroundTexture() const;

    std::map<std::string, SpriteSheet> m_spriteSheets;

    class LayerDrawable : public sf::Drawable, private sf::NonCopyable
    {
    public:
        LayerDrawable(TextureResource& tr, sf::Shader& shader);
        ~LayerDrawable() = default;

        void addPart(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textureName);
        void addSprite(const std::string& textureName, const SpriteSheet::Quad& frame);
        void buildShadow(sf::Shader& blurShader);
    private:
        struct LayerData
        {
            sf::Texture diffuseTexture;
            sf::Texture normalTexture;
            sf::VertexArray vertexArray;
        };

        sf::Shader& m_shader;
        std::map<std::string, LayerData> m_layerData;
        TextureResource& m_textureResource;

        std::unique_ptr<sf::RenderTexture> m_shadowTexture;
        sf::Sprite m_shadowSprite;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    } m_solidDrawable, m_rearDrawable, m_frontDrawable;
};

#endif //MAP_CONTROLLER_H_