#include <onut/Color.h>
#include <onut/Input.h>
#include <onut/Font.h>
#include <onut/PrimitiveBatch.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>
#include <onut/Vector2.h>
#include <onut/VertexBuffer.h>

static const float ZOOM = 32.0f;
static const int MAP_SIZE = 16;

static const uint8_t MAP[MAP_SIZE * MAP_SIZE] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
    1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,
    1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,
    2,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,
    1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,
    1,1,1,0,1,1,1,0,1,1,1,1,0,0,0,3,
    1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,
    1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,1,
    1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,
    1,1,1,1,1,0,0,0,1,1,0,0,1,1,1,1,
    1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,
    1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,
    1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

static const int TILE_PATH = 0;
static const int TILE_GRASS = 1;
static const int TILE_START = 2;
static const int TILE_END = 3;

static const int TOWER_NONE = 0;
static const int TOWER_ARROW = 1;

struct Node
{
    Vector2 position;
    int nextIndex;
    std::vector<Node*> children;
};

Node nodes[] = {
    { {-0.5f, 5.5f}, 0 },
    { {3.5f, 5.5f}, 0 },
    { {3.5f, 8.5f}, 0 },
    { {7.5f, 8.5f}, 0 },
    { {7.5f, 5.5f}, 0 },
    { {6.5f, 5.5f}, 0 },
    { {6.5f, 2.5f}, 0 },
    { {12.5f, 2.5f}, 0 },
    { {12.5f, 7.5f}, 0 },
    { {16.5f, 7.5f}, 0 },
    { {7.5f, 11.5f}, 0 },
    { {5.5f, 11.5f}, 0 },
    { {5.5f, 14.5f}, 0 },
    { {11.5f, 14.5f}, 0 },
    { {11.5f, 11.5f}, 0 },
    { {10.5f, 11.5f}, 0 },
    { {10.5f, 9.5f}, 0 },
    { {12.5f, 9.5f}, 0 }
};

struct Vertex
{
    Vector2 position; 
    Color color;
};

typedef void(*DRAW_SHAPE_FN)(OPrimitiveBatch* pPrimitiveBatch, const Vector2& position, float size, const Color& color);
typedef void(*SPAWN_FN)(Node* pNode);

struct Critter
{
    Vector2 position;
    float speed;
    float health;
    float baseHealth;
    Color color;
    float size;
    DRAW_SHAPE_FN drawShape;
    Node* pTargetNode;
    int value;
};

struct Spawner
{
    float interval;
    float delay;
    SPAWN_FN spawn;
    int count;
};

struct SpawnerDef
{
    float time;
    float interval;
    SPAWN_FN spawn;
    int count;
};

struct Wave
{
    std::vector<SpawnerDef> spawnerDefs;
};

void spawnPeasant(Node* pNode);
void spawnWarrior(Node* pNode);

static const std::vector<Wave> WAVES = {
    { 
        {
            { 0.0f, 1.0f, spawnPeasant, 20 }
        }
    },
    {
        {
            { 0.0f, 0.0f, spawnWarrior, 1 },
            { 0.5f, 0.75f, spawnPeasant, 6 },
            { 6.0f, 0.0f, spawnWarrior, 1 },
            { 6.5f, 0.75f, spawnPeasant, 6 },
            { 12.0f, 0.0f, spawnWarrior, 1 },
            { 12.5f, 0.75f, spawnPeasant, 6 },
            { 18.0f, 0.0f, spawnWarrior, 1 },
            { 18.5f, 0.75f, spawnPeasant, 6 }
        }
    }
};

// Resources
static OVertexBufferRef pMapVB;
static int mapVertCount;
static OShaderRef pLinesVS;
static OShaderRef pLinesPS;
static OTextureRef pScreenRT;
static OTextureRef pBloomRT;
static OFontRef pFont;

// Vars
static std::vector<Spawner> spawners;
static std::vector<Critter> critters;
static float waveTime = 0.0f;
static Wave wave = WAVES[0];
static int health = 20;
static int waveIndex= 0;
static float waveDelay = 0.0f;
static float cash = 200.0f;
static int placingTower = TOWER_NONE;
static Point placingPos;
static Matrix mapTransform;
static Matrix invMapTransform;
static uint8_t MAP_TOWERS[MAP_SIZE * MAP_SIZE] = { 0 };

void drawBox(OPrimitiveBatch* pPrimitiveBatch, const Vector2& position, float size, const Color& color)
{
    pPrimitiveBatch->draw(Vector2(position.x - size, position.y - size), color);
    pPrimitiveBatch->draw(Vector2(position.x + size, position.y - size), color);

    pPrimitiveBatch->draw(Vector2(position.x - size, position.y + size), color);
    pPrimitiveBatch->draw(Vector2(position.x + size, position.y + size), color);

    pPrimitiveBatch->draw(Vector2(position.x - size, position.y - size), color);
    pPrimitiveBatch->draw(Vector2(position.x - size, position.y + size), color);

    pPrimitiveBatch->draw(Vector2(position.x + size, position.y - size), color);
    pPrimitiveBatch->draw(Vector2(position.x + size, position.y + size), color);
}

void drawCircle(OPrimitiveBatch* pPrimitiveBatch, const Vector2& position, float size, const Color& color)
{
    float step = O2PI / 12.0f;
    for (float i = 0.0f; i < O2PI; i += step)
    {
        pPrimitiveBatch->draw(Vector2(
            position.x + std::cosf(i) * size, 
            position.y + std::sinf(i) * size), 
            color);
        pPrimitiveBatch->draw(Vector2(
            position.x + std::cosf(i + step) * size,
            position.y + std::sinf(i + step) * size),
            color);
    }
}

Node* getNextNode(Node* pNode)
{
    auto pRet = pNode->children[pNode->nextIndex];
    pNode->nextIndex = (pNode->nextIndex + 1) % pNode->children.size();
    return pRet;
}

void spawnPeasant(Node* pNode)
{
    critters.push_back({
        pNode->position,
        45.0f / 60.0f,
        30.0f, 30.0f,
        OColorHex(fbf236),
        0.15f,
        drawCircle,
        getNextNode(pNode),
        10
    });
}

void spawnWarrior(Node* pNode)
{
    critters.push_back({
        pNode->position,
        45.0f / 60.0f,
        50.0f, 50.0f,
        OColorHex(df7126),
        0.20f,
        drawCircle,
        getNextNode(pNode),
        10
        });
}

void initSettings()
{
    oSettings->setGameName("Data Oriented Tower Defense");
    oSettings->setIsFixedStep(false);
    oSettings->setResolution({800, 600});
    oSettings->setIsResizableWindow(true);
}

void init()
{
    // Load resources
    pLinesVS = OGetShader("lines.vs");
    pLinesPS = OGetShader("lines.ps");
    pScreenRT = OTexture::createScreenRenderTarget();
    pBloomRT = OTexture::createScreenRenderTarget(true);
    pFont = OGetFont("font.fnt");

    // Link nodes
    nodes[0].children.push_back(&nodes[1]);
    nodes[1].children.push_back(&nodes[2]);
    nodes[2].children.push_back(&nodes[3]);
    nodes[3].children.push_back(&nodes[4]);
    nodes[3].children.push_back(&nodes[10]);
    nodes[4].children.push_back(&nodes[5]);
    nodes[5].children.push_back(&nodes[6]);
    nodes[6].children.push_back(&nodes[7]);
    nodes[7].children.push_back(&nodes[8]);
    nodes[8].children.push_back(&nodes[9]);
    nodes[10].children.push_back(&nodes[11]);
    nodes[11].children.push_back(&nodes[12]);
    nodes[12].children.push_back(&nodes[13]);
    nodes[13].children.push_back(&nodes[14]);
    nodes[14].children.push_back(&nodes[15]);
    nodes[15].children.push_back(&nodes[16]);
    nodes[16].children.push_back(&nodes[17]);
    nodes[17].children.push_back(&nodes[8]);

    // Build map
    {
        std::vector<Vertex> vertices;
        Color pathColor = OColorHex(5b6ee1);
        Color startColor = OColorHex(df7126);
        Color endColor = OColorHex(5fcde4);
        for (auto y = 0; y < MAP_SIZE; ++y)
        {
            for (auto x = 0; x < MAP_SIZE; ++x)
            {
                auto tile = MAP[y * MAP_SIZE + x];
                if (tile != TILE_GRASS)
                {
                    if (MAP[(y - 1) * MAP_SIZE + x] == TILE_GRASS)
                    {
                        vertices.push_back({{(float)x, (float)y}, pathColor});
                        vertices.push_back({{(float)x + 1.0f, (float)y}, pathColor});
                    }
                    if (MAP[(y + 1) * MAP_SIZE + x] == TILE_GRASS)
                    {
                        vertices.push_back({{(float)x, (float)y + 1.0f}, pathColor});
                        vertices.push_back({{(float)x + 1.0f, (float)y + 1.0f}, pathColor});
                    }
                    if (x > 0 && MAP[y * MAP_SIZE + x - 1] == TILE_GRASS)
                    {
                        vertices.push_back({{(float)x, (float)y}, pathColor});
                        vertices.push_back({{(float)x, (float)y + 1.0f}, pathColor});
                    }
                    if (x < MAP_SIZE - 1 && MAP[y * MAP_SIZE + x + 1] == TILE_GRASS)
                    {
                        vertices.push_back({{(float)x + 1.0f, (float)y}, pathColor});
                        vertices.push_back({{(float)x + 1.0f, (float)y + 1.0f}, pathColor});
                    }
                }
                if (tile == TILE_START)
                {
                    auto startPos = Vector2((float)x, (float)y + 0.5f);
                    vertices.push_back({{startPos.x + 0.2f, startPos.y - .3f}, startColor});
                    vertices.push_back({{startPos.x + 0.2f, startPos.y + .3f}, startColor});
                    vertices.push_back({{startPos.x + 0.2f, startPos.y + .3f}, startColor});
                    vertices.push_back({{startPos.x + 0.5f, startPos.y}, startColor});
                    vertices.push_back({{startPos.x + 0.5f, startPos.y}, startColor});
                    vertices.push_back({{startPos.x + 0.2f, startPos.y - .3f}, startColor});
                }
                else if (tile == TILE_END)
                {
                    auto endPos = Vector2((float)x + 1.0f, (float)y + 0.5f);
                    vertices.push_back({{endPos.x - 0.5f, endPos.y - .3f}, endColor});
                    vertices.push_back({{endPos.x - 0.5f, endPos.y + .3f}, endColor});
                    vertices.push_back({{endPos.x - 0.5f, endPos.y + .3f}, endColor});
                    vertices.push_back({{endPos.x - 0.2f, endPos.y}, endColor});
                    vertices.push_back({{endPos.x - 0.2f, endPos.y}, endColor});
                    vertices.push_back({{endPos.x - 0.5f, endPos.y - .3f}, endColor});
                }
            }
        }
        mapVertCount = (int)vertices.size();
        pMapVB = OVertexBuffer::createStatic(vertices.data(), sizeof(Vertex) * mapVertCount);
    }
}

void updateWave(float dt)
{
    waveTime += dt;

    if (!wave.spawnerDefs.empty())
    {
        const auto& spawnerDef = wave.spawnerDefs.front();
        if (waveTime >= spawnerDef.time)
        {
            spawners.push_back({
                spawnerDef.interval,
                0.0f,
                spawnerDef.spawn,
                spawnerDef.count
                });
            wave.spawnerDefs.erase(wave.spawnerDefs.begin());
        }
    }

    for (size_t i = 0, len = spawners.size(); i < len;)
    {
        auto& spawner = spawners[i];
        spawner.delay -= dt;
        if (spawner.delay < 0.0f)
        {
            spawner.delay += spawner.interval;
            spawner.spawn(&nodes[0]);
            --spawner.count;
            if (spawner.count == 0)
            {
                spawners[i] = spawners.back();
                spawners.pop_back();
                --len;
                continue;
            }
        }
        ++i;
    }

    for (size_t i = 0, len = critters.size(); i < len;)
    {
        auto& critter = critters[i];
        auto dir = critter.pTargetNode->position - critter.position;
        auto dist = dir.Length();
        dir.Normalize();
        critter.position += dir * dt;
        dist -= critter.speed * dt;
        if (dist <= 0.0f)
        {
            if (critter.pTargetNode->children.empty())
            {
                critters.erase(critters.begin() + i);
                --health;
                --len;
                continue;
            }
            auto prev = critter.pTargetNode->position;
            critter.pTargetNode = getNextNode(critter.pTargetNode);
            dir = critter.pTargetNode->position - prev;
            dir.Normalize();
            critter.position = prev + dir * -dist;
        }
        ++i;
    }

    if (critters.empty() && spawners.empty())
    {
        waveDelay -= dt;
        if (waveDelay <= 0.0f)
        {
            waveDelay = 10.0f;
            ++waveIndex;
            if (waveIndex < (int)WAVES.size())
            {
                wave = WAVES[waveIndex];
                waveTime = 0.0f;
            }
        }
    }
}

void update()
{
    auto resolution = OScreenf;
    auto dt = ODT;

    // Setup transform
    mapTransform =
        Matrix::CreateTranslation(-(float)MAP_SIZE * 0.5f, -(float)MAP_SIZE * 0.5f, 0.0f) *
        Matrix::CreateScale(ZOOM) *
        Matrix::CreateTranslation(resolution.x * 0.5f, resolution.y * 0.5f, 0.0f);
    invMapTransform = mapTransform.Invert();

    auto simCount = 1;
    if (OInputPressed(OKeyTab)) simCount = 4;
    for (auto i = 0; i < simCount; ++i)
    {
        updateWave(dt);
    }

    if (OInputJustPressed(OKey1))
    {
        placingTower = TOWER_ARROW;
    }

    if (placingTower != TOWER_NONE)
    {
        if (OInputJustPressed(OKeyEscape) || OInputJustPressed(OMouse2))
        {
            placingTower = TOWER_NONE;
        }
        else
        {
            placingPos = { -1, -1 };
            auto mousePosOnMap = Vector2::Transform(oInput->mousePosf, invMapTransform);
            if (mousePosOnMap.x >= 0.0f && mousePosOnMap.y >= 0.0f && mousePosOnMap.x < (float)MAP_SIZE && mousePosOnMap.y < (float)MAP_SIZE)
            {
                placingPos.x = (int)mousePosOnMap.x;
                placingPos.y = (int)mousePosOnMap.y;
            }
        }
    }
}

void render()
{
    auto pRenderer = oRenderer.get();
    auto& renderStates = pRenderer->renderStates;
    auto pPrimitiveBatch = oPrimitiveBatch.get();
    auto resolution = OScreenf;
    auto pSpriteBatch = oSpriteBatch.get();

    pRenderer->clear(Color::Black);

    renderStates.renderTarget.push(pScreenRT);
    pRenderer->clear(Color::Black);

    // Set our drawing states
    renderStates.vertexShader = pLinesVS;
    renderStates.pixelShader = pLinesPS;
    renderStates.primitiveMode = OPrimitiveLineList;  
    renderStates.vertexBuffer = pMapVB;
    renderStates.blendMode = OBlendPreMultiplied;

    // Draw map
    {
        renderStates.world = mapTransform;
        pRenderer->draw(mapVertCount);
    }

    // Debug, draw path
    //Color pathColor = OColorHex(d77bba);
    //pPrimitiveBatch->begin(OPrimitiveLineList, nullptr, mapTransform);
    //for (const auto& node : nodes)
    //{
    //    for (auto pChild : node.children)
    //    {
    //        pPrimitiveBatch->draw(node.position, pathColor);
    //        pPrimitiveBatch->draw(pChild->position, pathColor);
    //    }
    //}
    //pPrimitiveBatch->end();

    // Draw critters
    {
        pPrimitiveBatch->begin(OPrimitiveLineList, nullptr, mapTransform);
        for (const auto& critter : critters)
        {
            critter.drawShape(pPrimitiveBatch, critter.position, critter.size, critter.color);
        }
        pPrimitiveBatch->end();
    }

    // Cursor
    {
        pPrimitiveBatch->begin(OPrimitiveLineList, nullptr, mapTransform);
        if (placingTower != TOWER_NONE && placingPos.x != -1 && placingPos.y != -1)
        {
            drawBox(pPrimitiveBatch, Vector2((float)placingPos.x + 0.5f, (float)placingPos.y + 0.5f), 0.4f, OColorHex(6abe30));
        }
        pPrimitiveBatch->end();
    }

    // HUD
    {
        Color availColor = OColorHex(6abe30);
        Color unavailColor = OColorHex(ac3232);
        pSpriteBatch->begin();
        pFont->draw("Wave: " + std::to_string(waveIndex + 1), Vector2(20, 20), OTopLeft, Color(0.75f));
        pFont->draw(std::to_string(health), Vector2(resolution.x - 40, 20), OTopRight, Color(0.75f));
        pFont->draw(std::to_string((int)cash), Vector2(resolution.x * 0.5f, 20), OTopRight, Color(0.75f));
        pFont->draw("(1) Arrow Tower 100$", Vector2(20, 100), OLeft, cash >= 100.0f ? availColor : unavailColor);
        pSpriteBatch->end();

        // Little heart and cash icon
        Color heartColor = OColorHex(ac3232);
        Color cashColor = OColorHex(fbf236);
        pPrimitiveBatch->begin(OPrimitiveLineList);
        {
            pPrimitiveBatch->draw(Vector2(resolution.x - 20, 20), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 15, 15), heartColor);

            pPrimitiveBatch->draw(Vector2(resolution.x - 15, 15), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 10, 20), heartColor);

            pPrimitiveBatch->draw(Vector2(resolution.x - 10, 20), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 20, 30), heartColor);

            pPrimitiveBatch->draw(Vector2(resolution.x - 20, 30), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 30, 20), heartColor);

            pPrimitiveBatch->draw(Vector2(resolution.x - 30, 20), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 25, 15), heartColor);

            pPrimitiveBatch->draw(Vector2(resolution.x - 25, 15), heartColor);
            pPrimitiveBatch->draw(Vector2(resolution.x - 20, 20), heartColor);
        }
        {
            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 15, 15), cashColor);
            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 5, 20), cashColor);

            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 5, 20), cashColor);
            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 15, 25), cashColor);

            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 15, 25), cashColor);
            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 5, 30), cashColor);

            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 10, 15), cashColor);
            pPrimitiveBatch->draw(Vector2(resolution.x * 0.5f + 10, 30), cashColor);
        }
        pPrimitiveBatch->end();
    }

    renderStates.renderTarget.pop();

    // Generate bloom
    {
        renderStates.renderTarget.push(pBloomRT);
        pSpriteBatch->begin();
        pSpriteBatch->drawRect(pScreenRT, OScreenRectf);
        pSpriteBatch->end();
        renderStates.renderTarget.pop();
        pBloomRT->blur(6.0f);
    }

    pSpriteBatch->begin();
    pSpriteBatch->drawRect(pScreenRT, OScreenRectf);
    renderStates.blendMode = OBlendAdd;
    pSpriteBatch->drawRect(pBloomRT, OScreenRectf, Color(2.0f));
    pSpriteBatch->end();
}

void renderUI()
{

}

void postRender()
{

}
