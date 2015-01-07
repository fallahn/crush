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

#include <MapController.hpp>
#include <Map.hpp>
#include <Node.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <map>

namespace
{
    const float spawnGapTime = 22.f;//time between spawns
    sf::Vector2f blockTextureSize;
}

MapController::MapController(CommandStack& cs, TextureResource& tr, ShaderResource& sr)
    : m_commandStack    (cs),
    m_itemTime          (spawnGapTime),
    m_itemActive        (false),
    m_textureResource   (tr),
    m_shaderResource    (sr),
    m_itemSprite        (tr.get("res/textures/item.png")),
    m_blockSprite       (tr.get("res/textures/steel_crate_diffuse.png")),
    m_solidDrawable     (tr, sr.get(Shader::Type::NormalMap)),
    m_rearDrawable      (tr, sr.get(Shader::Type::NormalMap)),
    m_frontDrawable     (tr, sr.get(Shader::Type::NormalMapSpecular))
{
    //TODO load textures based on map data
    //scale sprite to match node size
    blockTextureSize = sf::Vector2f(m_blockSprite.getTexture()->getSize());
    
    m_itemSprite.setFrameCount(16u);
    m_itemSprite.setFrameRate(18.f);
    m_itemSprite.setFrameSize({ 64, 64 });
    m_itemSprite.setLooped(true);
    m_itemSprite.setNormalMap(tr.get("res/textures/item_normal.png"));
    m_itemSprite.setShader(sr.get(Shader::Type::NormalMapSpecular));
    m_itemSprite.play();

    m_blockSprite.setFrameCount(1u);
    //m_blockSprite.setFrameSize({ 66, 66 });
    m_blockSprite.setNormalMap(tr.get("res/textures/steel_crate_normal.tga"));
    m_blockSprite.setShader(sr.get(Shader::Type::NormalMapSpecular));
}

//public
void MapController::update(float dt)
{
    m_itemTime -= dt;
    if (m_items.size() && m_itemTime < 0)
    {
        //spawn a new item if none active
        if(!m_itemActive)
        {
            const auto& item = m_items.back();
            spawn(item.node);
            m_itemTime = item.lifeTime;
            //m_items.pop_back();
            shuffleItems();

            m_itemActive = true;
        }
        //or deactivate
        else
        {
            //technically this despawns all bonuses - but there should only ever be one
            //present in the scene at a time
            Command c;
            c.categoryMask |= Category::Item;
            c.action = [](Node& n, float dt)
            {
                Event evt;
                evt.type = Event::Node;
                evt.node.action = Event::NodeEvent::Despawn;
                evt.node.type = Category::Item;
                auto pos = n.getCollisionBody()->getCentre();
                evt.node.positionX = pos.x;
                evt.node.positionY = pos.y;
                n.raiseEvent(evt);
            };
            m_commandStack.push(c);

            m_itemTime = spawnGapTime;
            m_itemActive = false;
        }
    }

    //update animations
    m_itemSprite.update(dt);

    for (auto& w : m_waterDrawables)
        w.update(dt);
}

void MapController::setSpawnFunction(std::function<void(const Map::Node&)>& func)
{
    spawn = func;
}

void MapController::loadMap(const Map& map)
{
    const auto& nodes = map.getNodes();
    for (const auto& n : nodes)
    {
        if (n.type == Category::Item)
        {
            m_items.emplace_back(n, Util::Random::value(12.f, 19.f));
        }
        else
        {
            spawn(n);
            switch(n.type )
            {
            case Category::Solid:
                //TODO select a single texture for solid blocks in editor
                m_solidDrawable.addPart(n.position, n.size, "funt"); 
                break;

            case Category::Block:
                m_blockSprite.setScale(n.size.x / blockTextureSize.x, n.size.y / blockTextureSize.y);
                break;
            case Category::Detail:
            {
                assert(!n.spriteSheet.empty());
                auto result = m_spriteSheets.find(n.spriteSheet);
                if (result == m_spriteSheets.end())
                {
                    //insert new sprite sheet
                    const std::string sheetName = n.spriteSheet.substr(0, n.spriteSheet.find("png")) + "json";
                    m_spriteSheets.insert(std::make_pair(n.spriteSheet, SpriteSheet("res/textures/atlases/" + sheetName)));
                    result = m_spriteSheets.find(n.spriteSheet); //hackiness to avoid default ctor for SpriteSheet
                }

                if (n.layer == Scene::RearDetail)
                {
                    m_rearDrawable.addSprite(n.spriteSheet, result->second.getFrame(n.image, n.position));
                }
                else
                {
                    //safe to assume layer is front? might not be....
                    m_frontDrawable.addSprite(n.spriteSheet, result->second.getFrame(n.image, n.position));
                }
            }
                break;
            default: break;
            }

        }

    }
    //shuffle item order
    shuffleItems();

    //TODO load background texture based on map data
    m_backgroundSprite.setTexture(m_textureResource.get("res/textures/background.png"));
    m_backgroundSprite.setFrameSize(sf::Vector2i(m_backgroundSprite.getTexture()->getSize()));
    m_backgroundSprite.setNormalMap(m_textureResource.get("res/textures/background_normal.png"));
    m_backgroundSprite.setShader(m_shaderResource.get(Shader::Type::NormalMap));
    m_shaderResource.get(Shader::Type::Water).setParameter("u_reflectMap", *m_backgroundSprite.getTexture());
}

sf::Drawable* MapController::getDrawable(MapController::MapDrawable type)
{
    switch (type) 
    {
    case MapDrawable::Solid:
        return static_cast<sf::Drawable*>(&m_solidDrawable);
    case MapDrawable::Item:
        return static_cast<sf::Drawable*>(&m_itemSprite);
    case MapDrawable::Block: //TODO random different textures?
        return static_cast<sf::Drawable*>(&m_blockSprite);
    case MapDrawable::Water:
        m_waterDrawables.emplace_back(m_textureResource.get("res/textures/water_normal.png"), m_shaderResource.get(Shader::Type::Water));
        return static_cast<sf::Drawable*>(&m_waterDrawables.back());
    case MapDrawable::RearDetail:
        return static_cast<sf::Drawable*>(&m_rearDrawable);
    case MapDrawable::FrontDetail:
        return static_cast<sf::Drawable*>(&m_frontDrawable);
    case MapDrawable::Background:
        return static_cast<sf::Drawable*>(&m_backgroundSprite);
    default: return nullptr;
    }
}

//private
void MapController::shuffleItems()
{
    std::random_shuffle(m_items.begin(), m_items.end(),
        [&](int i)
    {
        return Util::Random::value(0, m_items.size()) % i;
    });
}


//--------------drawable--------------
MapController::LayerDrawable::LayerDrawable(TextureResource& tr, sf::Shader& shader)
    : m_textureResource (tr),
    m_shader            (shader)
{

}

void MapController::LayerDrawable::addPart(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& textureName)
{
    if (m_layerData.find(textureName) == m_layerData.end())
    {
        auto pair = std::make_pair(textureName, LayerData());
        
        //TODO load actual texture name
        pair.second.diffuseTexture = m_textureResource.get("res/textures/brick_diffuse.png");
        pair.second.diffuseTexture.setRepeated(true);
        pair.second.normalTexture = m_textureResource.get("res/textures/brick_normal.png");
        pair.second.normalTexture.setRepeated(true);
        pair.second.vertexArray.setPrimitiveType(sf::Quads);

        m_layerData.insert(pair);
    }
    
    auto& vertexArray = m_layerData[textureName].vertexArray;
    vertexArray.append({ pos, pos });
    sf::Vector2f p(pos.x + size.x, pos.y);
    vertexArray.append({ p, p });
    p.y += size.y;
    vertexArray.append({ p, p });
    p.x -= size.x;
    vertexArray.append({ p, p });
}

void MapController::LayerDrawable::addSprite(const std::string& textureName, const SpriteSheet::Quad& frame)
{
    if (m_layerData.find(textureName) == m_layerData.end())
    {
        auto pair = std::make_pair(textureName, LayerData());

        pair.second.diffuseTexture = m_textureResource.get("res/textures/atlases/" + textureName);
        std::string normalName = textureName;
        normalName.insert(normalName.find(".png"), "_normal");
        pair.second.normalTexture = m_textureResource.get("res/textures/atlases/" + normalName);
        pair.second.vertexArray.setPrimitiveType(sf::Quads);

        m_layerData.insert(pair);
    }

    for (const auto& q : frame)
    {
        m_layerData[textureName].vertexArray.append(q);
    }
}

void MapController::LayerDrawable::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    m_shader.setParameter("u_inverseWorldViewMatrix", states.transform.getInverse());
    states.shader = &m_shader;

    for (const auto& layer : m_layerData)
    {
        m_shader.setParameter("u_diffuseMap", sf::Shader::CurrentTexture);
        m_shader.setParameter("u_normalMap", layer.second.normalTexture);
        states.texture = &layer.second.diffuseTexture;
        rt.draw(layer.second.vertexArray, states);
    }
}