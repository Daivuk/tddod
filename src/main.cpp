#if defined(WIN32)
#include <windows.h>
#endif

#include <GL/gl3w.h>
#include <SDL.h>

#include <chrono>
#include <stdio.h>

#include <entt/entt.hpp>

#include "components.h"
#include "constants.h"
#include "data.h"
#include "debugView.h"
#include "labels.h"

// Systems
#include "systems/CooldownSystem.h"
#include "systems/ColorAnimSystem.h"
#include "systems/DeathTimerSystem.h"
#include "systems/EndWaveCheckSystem.h"
#include "systems/EndWaveDelaySystem.h"
#include "systems/HealthBarRenderSystem.h"
#include "systems/InvestmentSystem.h"
#include "systems/KillSystem.h"
#include "systems/LineAnimSystem.h"
#include "systems/LineRenderSystem.h"
#include "systems/PhysicSystem.h"
#include "systems/PointRenderSystem.h"
#include "systems/ShapeRenderSystem.h"
#include "systems/SpawnSystem.h"
#include "systems/StaticLinesRenderSystem.h"
#include "systems/TargetingSystem.h"
#include "systems/TextRenderSystem.h"
#include "systems/TravelSystem.h"

// Helpers
#include "helpers/RenderingHelpers.h"

void init();
void updateSim(float dt);
void update(float dt);
void render(Registry &registry);

void onGunTurretClicked(Entity button);
void onSlowTurretClicked(Entity button);
void onRocketTurretClicked(Entity button);
void onCannonTurretClicked(Entity button);

static int updateSpeed = 1;
static Position mouse{ 0, 0 };
static Position worldMouse{ 0, 0 };
static Entity toolTip;
static Entity hoverEntity;
static Entity downButton;

#if defined(WIN32)
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
#else
int main(int argc, char** argv)
#endif
{
    // Init SDL
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    auto sdlWindow = SDL_CreateWindow(
        "Data Oriented Tower Defense",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    auto glContext = SDL_GL_CreateContext(sdlWindow);
    SDL_GL_SetSwapInterval(1);
    gl3wInit();

    init();

    // Main loop
    bool done = false;
    auto lastTime = std::chrono::high_resolution_clock::now();
    while (!done)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto diffTime = now - lastTime;
        lastTime = now;
        auto dt = (float)((double)std::chrono::duration_cast<std::chrono::microseconds>(diffTime).count() / 1000000.0);

        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_TAB) updateSpeed = 10;
                    else if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        if (updateSpeed) updateSpeed = 0;
                        else updateSpeed = 1;
                    }
                    else if (event.key.keysym.sym == SDLK_d)
                    {
                        g_debugViewEnabled = !g_debugViewEnabled;
                        if (g_debugViewEnabled)
                        {
                            int x, y;
                            SDL_GetWindowPosition(sdlWindow, &x, &y);
                            SDL_SetWindowSize(sdlWindow, WIDTH * 2, HEIGHT);
                            SDL_SetWindowPosition(sdlWindow, x - WIDTH / 2, y);
                        }
                        else
                        {
                            int x, y;
                            SDL_GetWindowPosition(sdlWindow, &x, &y);
                            SDL_SetWindowSize(sdlWindow, WIDTH, HEIGHT);
                            SDL_SetWindowPosition(sdlWindow, x + WIDTH / 2, y);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_u)
                    {
                        if (hoverEntity != NULL_ENTITY)
                        {
                            if (g_registry.has<Upgradable>(hoverEntity))
                            {
                                const auto& upgradable = g_registry.get<Upgradable>(hoverEntity);
                                g_registry.view<Player, Bank>().each([&upgradable](auto entity, const Player& player, Bank& bank)
                                {
                                    if (bank.amount >= upgradable.cost)
                                    {
                                        bank.amount -= upgradable.cost;
                                        upgradable.upgrade(hoverEntity);
                                    }
                                });
                            }
                        }
                    }
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_TAB) updateSpeed = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (hoverEntity != NULL_ENTITY)
                        {
                            downButton = hoverEntity;
                        }
                        else
                        {
                            g_registry.view<Placer, Position>().each([](auto entity, const Placer& placer, const Position& position)
                            {
                                placer.place(position);
                            });
                            g_registry.destroy<Placer>();
                        }
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        // Cancel placing turret
                        g_registry.destroy<Placer>();
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (downButton != NULL_ENTITY)
                        {
                            if (downButton == hoverEntity && g_registry.has<Button, ON_BUTTON_CLICK>(downButton))
                            {
                                auto buttonOnClick = g_registry.get<ON_BUTTON_CLICK>(downButton);
                                if (buttonOnClick) buttonOnClick(downButton);
                            }
                            downButton = NULL_ENTITY;
                        }
                    }
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    // Screen coords
                    mouse.x = (float)event.motion.x;
                    mouse.y = (float)event.motion.y;

                    // Convert to world coords
                    worldMouse.x = (mouse.x - (WIDTH - MAP_WIDTH * ZOOM) / 2.0f) / ZOOM + 0.5f;
                    worldMouse.y = (mouse.y - (HEIGHT - MAP_HEIGHT * ZOOM) / 2.0f) / ZOOM + 0.4f;
                    break;
                }
            }
        }

        // Update game
        for (int i = 0; i < updateSpeed; ++i)
        {
            updateSim(dt);
        }
        update(dt);

        // Draw game
        render(g_registry);

        // Swap buffers
        SDL_GL_SwapWindow(sdlWindow);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

struct Sound
{

};

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
    //len = (len > audio_len ? audio_len : len);
    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    //audio_pos += len;
    //audio_len -= len;
}

void init()
{
    // Load sounds
    static Uint32 wav_length; // length of our sample
    static Uint8 *wav_buffer; // buffer containing our audio file
    static SDL_AudioSpec wav_spec; // the specs of our piece of music
                                   /* Load the WAV */
                                   // the specs, length and buffer of our wav are filled
    if (SDL_LoadWAV("assets/gun.wav", &wav_spec, &wav_buffer, &wav_length) == NULL)
    {
        assert(false);
        return;
    }
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    if (SDL_OpenAudio(&wav_spec, NULL) < 0)
    {
        assert(false);
        return;
    }
    SDL_PauseAudio(0);

    // Load resources
    {
        auto entity = g_registry.create();
        g_registry.assign<Label::RESOURCES>(entt::tag_t{}, entity);

        Resources resources;
        resources.programPC = Rendering::createProgram(PC_VS, PC_PS);
        resources.programPTC = Rendering::createProgram(PTC_VS, PTC_PS);
        resources.fontTexture = Rendering::createTexture("assets/font.png");
        resources.vertexBuffer = Rendering::createVertexBuffer();
        resources.pPCVertices = new VertexPC[MAX_VERTICES];
        resources.pPTCVertices = new VertexPTC[MAX_VERTICES];
        g_registry.assign<Resources>(entity, resources);
    }

    downButton = NULL_ENTITY;

    // Create map entity
    {
        auto entity = g_registry.create();
        g_registry.assign<StaticLinesRenderer>(entity, 
            Rendering::createVertexBuffer(sizeof(MAP_VERTS), MAP_VERTS),
            (GLsizei)(sizeof(MAP_VERTS) / (sizeof(float) * 6)));
    }

    // Wave manager that starts at first wave
    {
        Entity waveLabel;
        {
            auto entity = g_registry.create();
            g_registry.assign<Position>(entity, Position{ 0, 0 });
            g_registry.assign<Color>(entity, Color{ 0.75f, 0.75f, 0.75f, 1 });
            g_registry.assign<TextRenderer>(entity, "", Color{ 0,0,0,0 }, 0.0f);
            waveLabel = entity;
        }

        // Waves controller
        WavesController wavesController;
        {
            auto entity = g_registry.create();
            g_registry.assign<Label::WAVES_CONTROLLER>(entt::tag_t{}, entity);
            g_registry.assign<EndWaveChecker>(entity);
            wavesController = g_registry.assign<WavesController>(entity, 0, WAVE_COUNT, (Wave*)WAVES, waveLabel);
        }

        // Spawner
        {
            auto entity = g_registry.create();
            g_registry.assign<Wave>(entity, Wave{ wavesController.pWaves[wavesController.currentWaveIndex] });
            g_registry.assign<Position>(entity, WAVE_START_POSITION);
            g_registry.assign<Spawner>(entity, WAVE_START_TIME);
        }
    }

    // Player
    {
        Entity cashLabel;
        {
            auto entity = g_registry.create();
            g_registry.assign<Position>(entity, Position{ 16, 0 });
            g_registry.assign<Color>(entity, Color{ 1, 1, 0, 1 });
            g_registry.assign<TextRenderer>(entity, "0$", Color{ 0,0,0,0 }, 1.0f);
            cashLabel = entity;
        }
        auto entity = g_registry.create();
        g_registry.assign<Player>(entity, cashLabel);
        g_registry.assign<Bank>(entity, 150.0f);
        g_registry.assign<Investor>(entity, 0.003f);
    }

    // Turret buttons
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, Position{ -1, 16.5f });
        g_registry.assign<Size>(entity, Size{ 2.5f, 0.4f });
        g_registry.assign<Color>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f });
        g_registry.assign<TextRenderer>(entity, "Gun Turret", Color{ 0,0,0,0 }, 0.5f);
        g_registry.assign<Name>(entity, "$200 - Ground and air");
        g_registry.assign<ShapeRenderer>(entity, drawBox);
        g_registry.assign<CashThresholdButtonToggler>(entity, 100.0f);
        g_registry.assign<ON_BUTTON_CLICK>(entity, onGunTurretClicked);
    }
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, Position{ 5, 16.5f });
        g_registry.assign<Size>(entity, Size{ 2.5f, 0.4f });
        g_registry.assign<Color>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f });
        g_registry.assign<TextRenderer>(entity, "Slow Turret", Color{ 0,0,0,0 }, 0.5f);
        g_registry.assign<Name>(entity, "$300 - Slows down in critters");
        g_registry.assign<ShapeRenderer>(entity, drawBox);
        g_registry.assign<CashThresholdButtonToggler>(entity, 200.0f);
        g_registry.assign<ON_BUTTON_CLICK>(entity, onSlowTurretClicked);
    }
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, Position{ 11, 16.5f });
        g_registry.assign<Size>(entity, Size{ 2.5f, 0.4f });
        g_registry.assign<Color>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f });
        g_registry.assign<TextRenderer>(entity, "Rocket Turret", Color{ 0,0,0,0 }, 0.5f);
        g_registry.assign<Name>(entity, "$400 - Air only");
        g_registry.assign<ShapeRenderer>(entity, drawBox);
        g_registry.assign<CashThresholdButtonToggler>(entity, 300.0f);
        g_registry.assign<ON_BUTTON_CLICK>(entity, onRocketTurretClicked);
    }
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, Position{ 17, 16.5f });
        g_registry.assign<Size>(entity, Size{ 2.5f, 0.4f });
        g_registry.assign<Color>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f });
        g_registry.assign<TextRenderer>(entity, "Cannon Turret", Color{ 0,0,0,0 }, 0.5f);
        g_registry.assign<Name>(entity, "$500 - Ground only with splash damage");
        g_registry.assign<ShapeRenderer>(entity, drawBox);
        g_registry.assign<CashThresholdButtonToggler>(entity, 400.0f);
        g_registry.assign<ON_BUTTON_CLICK>(entity, onCannonTurretClicked);
    }

    // Tool tip
    {
        auto entity = g_registry.create();
        toolTip = entity;
    }
}

void createPlacer(PLACE_FN place)
{
    // Clear previous, we only accept 1 placer
    g_registry.destroy<Placer>();

    auto entity = g_registry.create();
    g_registry.assign<Placer>(entity, place);
    g_registry.assign<Color>(entity, Color{ 0, 1, 1, 0.75f });
    g_registry.assign<Size>(entity, Size{ 0.45f, 0.45f });
    g_registry.assign<ShapeRenderer>(entity, drawBox);
    g_registry.assign<SizePulseAnim>(entity, 0.0f, 20.0f, 0.35f, 0.45f);
}

void onGunTurretClicked(Entity button)
{
    createPlacer(createGunTurret);
}

void onSlowTurretClicked(Entity button)
{
    createPlacer(createSlowTurret);
}

void onRocketTurretClicked(Entity button)
{
    createPlacer(createRocketTurret);
}

void onCannonTurretClicked(Entity button)
{
    createPlacer(createCannonTurret);
}

static void updateSim(float dt)
{
    updateSpawnSystem(g_registry, dt);
    updateEndWaveCheckSystem(g_registry);
    updateEndWaveDelaySystem(g_registry, dt);
    updateTravelSystem(g_registry, dt);
    updateInvestmentSystem(g_registry, dt);
    updateCooldownSystem(g_registry, dt);
    updateTargetingSystem(g_registry, dt);
    updateDeathTimerSystem(g_registry, dt);
    updateColorAnimSystem(g_registry, dt);
    updateLineAnimSystem(g_registry, dt);
    updatePhysicSystem(g_registry, dt);
    updateKillSystem(g_registry);

    // Animations on shape sizes (Pretty much only used for placement cursor)
    g_registry.view<SizePulseAnim, Size>().each([dt](auto entity, SizePulseAnim& sizePulseAnim, Size& size)
    {
        auto anim = sizePulseAnim.anim + sizePulseAnim.speed * dt;
        auto t = std::sinf(anim);
        auto r = (sizePulseAnim.to - sizePulseAnim.from) / 2.0f;
        auto sizeA = sizePulseAnim.from + r + t * r;
        sizePulseAnim.anim = anim;
        size.w = sizeA;
        size.h = sizeA;
    });
}

static void update(float dt)
{
    // Update waves HUD text
    g_registry.view<WavesController>().each([dt](auto entity, const WavesController &wavesController)
    {
        auto& textRenderer = g_registry.get<TextRenderer>(wavesController.waveLabel);
        textRenderer.text = "Wave: " + std::to_string(wavesController.currentWaveIndex + 1);
    });

    // Update player's hud
    auto totalCash = 0.0f;
    g_registry.view<Player, Bank>().each([&totalCash](auto entity, const Player& player, const Bank& bank)
    {
        totalCash += bank.amount;
        auto& textRenderer = g_registry.get<TextRenderer>(player.cashLabel);
        textRenderer.text = "Cash: $" + std::to_string((int)bank.amount);
    });

    // Find what the mouse is hovering, and show tool tip
    hoverEntity = NULL_ENTITY;
    {
        // Rect check
        auto view = g_registry.view<Name, Position, Size>();
        for (auto entity : view)
        {
            const auto& position = view.get<Position>(entity);
            const auto& size = view.get<Size>(entity);

            if (worldMouse.x >= position.x - size.w &&
                worldMouse.x <= position.x + size.w &&
                worldMouse.y >= position.y - size.h &&
                worldMouse.y <= position.y + size.h)
            {
                hoverEntity = entity;
            }
        }

        if (hoverEntity != NULL_ENTITY)
        {
            const auto text = g_registry.get<Name>(hoverEntity).pName;
            float w = (float)strlen(text) * TEXT_ADVANCE;
            float align = std::max(worldMouse.x + 0.5f + w - 21.0f, 0.0f) / w;

            g_registry.accommodate<Position>(toolTip, Position{ worldMouse.x + 0.5f, worldMouse.y + 0.5f });
            g_registry.accommodate<Color>(toolTip, Color{ 0.75f, 0.75f, 0.85f, 1.0f });
            g_registry.accommodate<TextRenderer>(toolTip, text, Color{ 0, 0.125f, 0.25f, 0.75f }, align);
        }
        else
        {
            g_registry.reset<TextRenderer>(toolTip);
        }
    }

    // Enable/Disable Buttons based on a cash threshold
    g_registry.view<CashThresholdButtonToggler, Color>().each([totalCash](auto entity, const CashThresholdButtonToggler& cashThresholdButtonToggler, Color& color)
    {
        if (cashThresholdButtonToggler.threshold < totalCash)
        {
            g_registry.accommodate<Button>(entity);
        }
        else
        {
            g_registry.reset<Button>(entity);
            color = { 0.3f, 0.3f, 0.3f, 1 };
        }
    });

    // Update mouse hover/down color for buttons
    g_registry.view<Button, Color>().each([](auto entity, const Button& button, Color& color)
    {
        if (entity == hoverEntity)
        {
            if (downButton == hoverEntity)
                color = { 0.5f, 0.5f, 0.4f, 1 };
            else
                color = { 1, 1, 0.8f, 1 };
        }
        else
        {
            if (downButton == entity)
                color = { 1, 1, 0.8f, 1 };
            else
                color = { 0.5f, 0.75f, 0.6f, 1 };
        }
    });

    // Mouse to place turret
    g_registry.view<Placer>().each([](auto entity, const Placer& placer)
    {
        int mx = (int)worldMouse.x;
        int my = (int)worldMouse.y;
        if (mx < 0 || mx >= MAP_WIDTH ||
            my < 0 || my >= MAP_HEIGHT)
        {
            g_registry.reset<Position>(entity);
        }
        else
        {
            g_registry.accommodate<Position>(entity, Position{ (float)mx + 0.5f, (float)my + 0.5f });
        }
    });
}

void spawnParticlesFromShape(const Position& position, const ShapeRenderer& shapeRenderer, const Size& size, const Color& color)
{
    Resources &resources = g_registry.get<Resources>(g_registry.attachee<Label::RESOURCES>());

    auto pVertices = resources.pPCVertices;
    pVertices += shapeRenderer.drawShape(pVertices, position, size.w, size.h, color);
    auto count = pVertices - resources.pPCVertices;
    for (int i = 0; i < count; i += 2)
    {
        const auto& vertex1 = resources.pPCVertices[i];
        const auto& vertex2 = resources.pPCVertices[i + 1];

        auto vdx = vertex2.position.x - vertex1.position.x;
        auto vdy = vertex2.position.y - vertex1.position.y;
        auto l = std::sqrtf(vdx * vdx + vdy * vdy);
        vdx = vdx / l * 0.025f;
        vdy = vdy / l * 0.025f;
        auto p = vertex1.position;

        for (float j = 0; j < l; j += 0.025f)
        {
            auto entity = g_registry.create();
            auto dx = p.x - position.x;
            auto dy = p.y - position.y;

            g_registry.assign<Position>(entity, p);
            g_registry.assign<PointRenderer>(entity);
            g_registry.assign<DeathTimer>(entity, 2.0f);
            g_registry.assign<Color>(entity, Color{ 1,1,1,1 });
            g_registry.assign<ColorAnim>(entity, 0.0f, 2.0f, vertex1.color, Color{ 0, 0, 0, 0 });
            g_registry.assign<Physics>(entity, p.x, p.y, 0.0f,
                dx * 8.0f + (float)(rand() % 1000 - 500) / 3000.0f,
                dy * 8.0f + (float)(rand() % 1000 - 500) / 3000.0f,
                1.0f + (float)(rand() % 1000 - 500) / 3000.0f);

            p.x += vdx;
            p.y += vdy;
        }
    }
}

void render(Registry &registry)
{
    Rendering::beginFrame(registry);

    Rendering::prepareForPC(registry);
    updateStaticLinesRenderSystem(registry);
    updateShapeRenderSystem(registry);
    updateLineRenderSystem(registry);
    updatePointRenderSystem(registry);
    updateHealthBarRenderSystem(registry);

    Rendering::prepareForPTC(registry);
    updateTextRenderSystem(registry);

    // Debug view
    if (g_debugViewEnabled)
    {
        glViewport(WIDTH, 0, WIDTH, HEIGHT);
        drawDebugView();
    }

    Rendering::endFrame();
}
