#include <onut/Color.h>
#include <onut/PrimitiveBatch.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
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
    { {20.5f, 14.5f}, 0 },
    { {20.5f, 11.5f}, 0 },
    { {19.5f, 11.5f}, 0 },
    { {19.5f, 9.5f}, 0 },
    { {21.5f, 9.5f}, 0 }
};

struct Vertex
{
    Vector2 position; 
    Color color;
};

struct Critter
{
    Vector2 position;
    float life;
    Color color;
};

// Resources
static OVertexBufferRef pMapVB;
static int mapVertCount;
static OShaderRef pLinesVS;
static OShaderRef pLinesPS;
static OTextureRef pScreenRT;
static OTextureRef pBloomRT;

void initSettings()
{
    oSettings->setGameName("Data Oriented Tower Defense");
    oSettings->setIsFixedStep(false);
    oSettings->setResolution({800, 600});
}

void init()
{
    // Load resources
    pLinesVS = OGetShader("lines.vs");
    pLinesPS = OGetShader("lines.ps");
    pScreenRT = OTexture::createScreenRenderTarget();
    pBloomRT = OTexture::createScreenRenderTarget(true);

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

void update()
{

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
    auto mapTransform = 
        Matrix::CreateTranslation(-(float)MAP_SIZE * 0.5f, -(float)MAP_SIZE * 0.5f, 0.0f) *
        Matrix::CreateScale(ZOOM) *
        Matrix::CreateTranslation(resolution.x * 0.5f, resolution.y * 0.5f, 0.0f);
    auto invMapTransform = mapTransform.Invert();

    // Draw map
    renderStates.world = mapTransform;
    pRenderer->draw(mapVertCount);

    // Debug, draw path
    pPrimitiveBatch->begin(OPrimitiveLineList, nullptr, mapTransform);
    for (const auto& node : nodes)
    {
    }
    pPrimitiveBatch->end();

    renderStates.renderTarget.pop();

    // Generate bloom
    {
        renderStates.renderTarget.push(pBloomRT);
        pSpriteBatch->begin();
        pSpriteBatch->drawRect(pScreenRT, OScreenRectf);
        pSpriteBatch->end();
        renderStates.renderTarget.pop();
        pBloomRT->blur(8.0f);
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
