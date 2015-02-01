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
#include <algorithm>

namespace
{
    const float spawnGapTime = 22.f;//time between spawns

    float hatSpawnTime = 0.f;
    sf::Uint8 hatSpawnCount = 10u;

    const sf::Uint8 blockTextureCount = 4u;
    sf::Vector2f blockTextureSize;
}

MapController::MapController(CommandStack& cs, TextureResource& tr, ShaderResource& sr)
    : m_commandStack    (cs),
    m_itemTime          (spawnGapTime),
    m_itemActive        (false),
    m_textureResource   (tr),
    m_shaderResource    (sr),
    m_itemSprite        (tr.get("res/textures/map/item.png")),
    m_hatSprite         (tr.get("res/textures/map/hat_diffuse.png")),
    m_hatCount          (0u),
    m_solidDrawable     (tr, sr.get(Shader::Type::NormalMap)),
    m_rearDrawable      (tr, sr.get(Shader::Type::NormalMap)),
    m_frontDrawable     (tr, sr.get(Shader::Type::NormalMapSpecular))
{
    //scale sprite to match node size
    blockTextureSize = sf::Vector2f(tr.get("res/textures/map/steel_crate_diffuse.png").getSize() / 2u); //KLUUUDDGGE!!!

    for (auto i = 0u; i < blockTextureCount; ++i)
    {
        m_blockSprites.emplace_back(tr.get("res/textures/map/steel_crate_diffuse.png"));
        auto& blockSprite = m_blockSprites.back();
        blockSprite.setFrameCount(1u);
        blockSprite.setNormalMap(tr.get("res/textures/map/steel_crate_normal.tga"));
        blockSprite.setShader(sr.get(Shader::Type::NormalMap));
        blockSprite.setFrameCount(blockTextureCount);
        blockSprite.setFrameSize(sf::Vector2i(blockTextureSize));
        blockSprite.play(Animation(i, i));
    }

    m_itemSprite.setFrameCount(16u);
    m_itemSprite.setFrameRate(18.f);
    m_itemSprite.setFrameSize({ 64, 64 });
    m_itemSprite.setLooped(true);
    m_itemSprite.setNormalMap(tr.get("res/textures/map/item_normal.png"));
    m_itemSprite.setShader(sr.get(Shader::Type::NormalMapSpecular));
    m_itemSprite.play();

    m_hatSprite.setFrameSize(sf::Vector2i(m_hatSprite.getTexture()->getSize()));
    m_hatSprite.setNormalMap(tr.get("res/textures/map/hat_normal.png"));
    m_hatSprite.setShader(sr.get(Shader::Type::Metal));

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

    //check for new hattage
    if (hatSpawnTime > 0)
    {
        hatSpawnTime -= dt;
        if (hatSpawnTime <= 0)
        {
            spawnHat();
        }
    }
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
            switch (n.type)
            {
            case Category::Solid:
                m_solidDrawable.addPart(n.position, n.size, map.getPlatformImageName());
                break;

            case Category::Block:
                //m_blockSprite.setScale(n.size.x / blockTextureSize.x, n.size.y / blockTextureSize.y);
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

    //load background texture based on map data
    std::string imageName = map.getBackgroundImageName();
    m_backgroundSprite.setTexture(m_textureResource.get("res/textures/map/" + imageName));
    m_backgroundSprite.setFrameSize(sf::Vector2i(m_backgroundSprite.getTexture()->getSize()));
    auto strpos = imageName.find_last_of('.');
    if(strpos != std::string::npos)
        imageName.insert(strpos, "_normal");
    m_backgroundSprite.setNormalMap(m_textureResource.get("res/textures/map/" + imageName));
    m_backgroundSprite.setShader(m_shaderResource.get(Shader::Type::NormalMap));
    m_shaderResource.get(Shader::Type::Water).setParameter("u_reflectMap", *m_backgroundSprite.getTexture());
    m_shaderResource.get(Shader::Type::WaterDrop).setParameter("u_reflectMap", *m_backgroundSprite.getTexture());
    m_shaderResource.get(Shader::Type::Metal).setParameter("u_reflectMap", *m_backgroundSprite.getTexture());

    //std::function<const sf::Texture&()> f = std::bind(&MapController::getBackgroundTexture, this);
    //Shader::UniformBinding::Ptr fb = std::make_unique<Shader::FunctionBinding<const sf::Texture&>>(m_shaderResource.get(Shader::Type::Metal), "u_reflectMap", f);
    //m_shaderResource.addBinding(fb);

    m_solidDrawable.buildShadow(m_shaderResource.get(Shader::Type::GaussianBlur));

    //generate some random hat spawns
    for(auto i = 0u; i < 40u; ++i)
    {
        m_hatSpawns.emplace_back(static_cast<float>(Util::Random::value(40, 1880)), static_cast<float>(Util::Random::value(-30, 1000)));
    }
    //remove points inside nodes
    for (const auto& n : nodes)
    {
        sf::FloatRect nodebox(n.position, n.size);
        m_hatSpawns.erase(std::remove_if(m_hatSpawns.begin(), m_hatSpawns.end(),
            [&nodebox](const sf::Vector2f& p)
        {
            return nodebox.contains(p);
        }), m_hatSpawns.end());
    }
    
    spawnHat();
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
        return static_cast<sf::Drawable*>(&m_blockSprites[Util::Random::value(0, blockTextureCount - 1)]);
    case MapDrawable::Water:
        m_waterDrawables.emplace_back(m_textureResource.get("res/textures/map/water_normal.png"), m_shaderResource.get(Shader::Type::Water));
        return static_cast<sf::Drawable*>(&m_waterDrawables.back());
    case MapDrawable::RearDetail:
        return static_cast<sf::Drawable*>(&m_rearDrawable);
    case MapDrawable::FrontDetail:
        return static_cast<sf::Drawable*>(&m_frontDrawable);
    case MapDrawable::Background:
        return static_cast<sf::Drawable*>(&m_backgroundSprite);
    case MapDrawable::Hat:
        return static_cast<sf::Drawable*>(&m_hatSprite);
    default: return nullptr;
    }
}

void MapController::onNotify(Subject& s, const Event& e)
{
    switch (e.type)
    {
    case Event::Node:
        switch (e.node.action)
        {
        case Event::NodeEvent::Despawn:
            if (e.node.type == Category::HatDropped
                || e.node.type == Category::HatCarried) //might be on head when killed
            {
                hatSpawnTime = static_cast<float>(Util::Random::value(20, 30));
                m_hatCount--;
            }
            break;
        default: break;
        }
        break;
    default: break;
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

void MapController::spawnHat()
{
    if (m_hatCount == 0u)
    {
        Map::Node n;
        n.position = m_hatSpawns[Util::Random::value(0, m_hatSpawns.size() - 1)];
        n.size = sf::Vector2f(m_hatSprite.getTexture()->getSize());
        spawn(n);
        m_hatCount++;
    }
}

const sf::Texture& MapController::getBackgroundTexture() const
{
    return *m_backgroundSprite.getTexture();
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
        
        std::string texture = textureName;
        pair.second.diffuseTexture = m_textureResource.get("res/textures/map/" + texture);
        pair.second.diffuseTexture.setRepeated(true);
        auto strpos = texture.find_last_of('.');
        if (strpos != std::string::npos)
            texture.insert(strpos, "_normal");

        pair.second.normalTexture = m_textureResource.get("res/textures/map/" + texture);
        pair.second.normalTexture.setRepeated(true); 
        pair.second.vertexArray.setPrimitiveType(sf::Quads);

        m_layerData.insert(pair);
    }
    
    auto& vertexArray = m_layerData[textureName].vertexArray;
    vertexArray.append({ pos, sf::Color::Black, pos });
    sf::Vector2f p(pos.x + size.x, pos.y);
    vertexArray.append({ p, sf::Color::Black, p });
    p.y += size.y;
    vertexArray.append({ p, sf::Color::Black, p });
    p.x -= size.x;
    vertexArray.append({ p, sf::Color::Black, p });
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

    for (auto& q : frame)
    {
        m_layerData[textureName].vertexArray.append(q);
    }
}

void MapController::LayerDrawable::buildShadow(sf::Shader& blurShader)
{
    const sf::Vector2i texSize(480, 270); //TODO link magic numbers to view size
    m_shadowTexture = std::make_unique<sf::RenderTexture>();
    m_shadowTexture->create(texSize.x, texSize.y);
    m_shadowTexture->setSmooth(true);

    sf::RenderStates states;
    states.transform.scale(0.25f, 0.25f);

    m_shadowTexture->clear(sf::Color::White);
    for (auto& l : m_layerData)
    {
        m_shadowTexture->draw(l.second.vertexArray, states);
    }
    m_shadowTexture->display();

    
    sf::RenderTexture tempRt;
    tempRt.create(texSize.x, texSize.y);
    for (auto i = 0u; i < 2u; ++i)
    {
        blurShader.setParameter("u_diffuse", m_shadowTexture->getTexture());
        blurShader.setParameter("u_offset", sf::Vector2f(1.f / texSize.x, 0.f));
        tempRt.clear(sf::Color::White);
        tempRt.draw(sf::Sprite(m_shadowTexture->getTexture()), &blurShader);
        tempRt.display();

        blurShader.setParameter("u_diffuse", tempRt.getTexture());
        blurShader.setParameter("u_offset", sf::Vector2f(0.f, 1.f / texSize.y));
        m_shadowTexture->clear(sf::Color::White);
        m_shadowTexture->draw(sf::Sprite(tempRt.getTexture()), &blurShader);
        m_shadowTexture->display();
    }

    m_shadowSprite.setTexture(m_shadowTexture->getTexture());
    m_shadowSprite.setScale(4.f, 4.f);
}

void MapController::LayerDrawable::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    if (m_shadowTexture)
    {
        //TODO multi pass for each light offset?
        rt.draw(m_shadowSprite, sf::BlendMultiply);
    }
    
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