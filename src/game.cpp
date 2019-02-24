#include "constants.h"
#include "debugView.h"
#include "game.h"
#include "labels.h"

// Components
#include "components/Bank.h"
#include "components/Hud.h"
#include "components/Inputs.h"
#include "components/Investor.h"
#include "components/Resources.h"
#include "components/StaticLinesRenderer.h"
#include "components/UIContext.h"
#include "components/Upgradable.h"

// Systems
#include "systems/ButtonStateSystem.h"
#include "systems/CashThresholdButtonToggleSystem.h"
#include "systems/ClickSystem.h"
#include "systems/CooldownSystem.h"
#include "systems/ColorAnimSystem.h"
#include "systems/DeathTimerSystem.h"
#include "systems/EndWaveCheckSystem.h"
#include "systems/EndWaveDelaySystem.h"
#include "systems/HealthBarRenderSystem.h"
#include "systems/HoverSystem.h"
#include "systems/HudSystem.h"
#include "systems/InputSystem.h"
#include "systems/InvestmentSystem.h"
#include "systems/KillSystem.h"
#include "systems/LineAnimSystem.h"
#include "systems/LineRenderSystem.h"
#include "systems/PhysicSystem.h"
#include "systems/PlaceSystem.h"
#include "systems/PointRenderSystem.h"
#include "systems/ShapeRenderSystem.h"
#include "systems/SizePulseAnimSystem.h"
#include "systems/SpawnSystem.h"
#include "systems/StaticLinesRenderSystem.h"
#include "systems/TargetingSystem.h"
#include "systems/TextRenderSystem.h"
#include "systems/ToolTipSystem.h"
#include "systems/TravelSystem.h"
#include "systems/UpgradeSystem.h"

// Helpers
#include "helpers/RenderingHelpers.h"
#include "helpers/TowerHelpers.h"
#include "helpers/UIHelpers.h"
#include "helpers/WaveHelpers.h"

namespace Game
{

    //struct Sound
    //{
    //
    //};
    //
    //void my_audio_callback(void *userdata, Uint8 *stream, int len)
    //{
    //    //len = (len > audio_len ? audio_len : len);
    //    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
    //
    //    //audio_pos += len;
    //    //audio_len -= len;
    //}

    void init(Registry &registry)
    {
        //// Load sounds
        //static Uint32 wav_length; // length of our sample
        //static Uint8 *wav_buffer; // buffer containing our audio file
        //static SDL_AudioSpec wav_spec; // the specs of our piece of music
        //                               /* Load the WAV */
        //                               // the specs, length and buffer of our wav are filled
        //if (SDL_LoadWAV("assets/gun.wav", &wav_spec, &wav_buffer, &wav_length) == NULL)
        //{
        //    assert(false);
        //    return;
        //}
        //wav_spec.callback = my_audio_callback;
        //wav_spec.userdata = NULL;
        //if (SDL_OpenAudio(&wav_spec, NULL) < 0)
        //{
        //    assert(false);
        //    return;
        //}
        //SDL_PauseAudio(0);

        // Load resources
        {
            auto entity = registry.create();
            registry.assign<Label::RESOURCES>(entt::tag_t{}, entity);

            Resources resources;
            resources.programPC = Rendering::createProgram(PC_VS, PC_PS);
            resources.programPTC = Rendering::createProgram(PTC_VS, PTC_PS);
            resources.fontTexture = Rendering::createTexture("assets/font.png");
            resources.vertexBuffer = Rendering::createVertexBuffer();
            resources.pPCVertices = new VertexPC[MAX_VERTICES];
            resources.pPTCVertices = new VertexPTC[MAX_VERTICES];
            registry.assign<Resources>(entity, resources);
        }

        // Create map entity
        {
            auto entity = registry.create();
            StaticLinesRenderer staticLineRenderer;
            staticLineRenderer.vertexBuffer = Rendering::createVertexBuffer(sizeof(MAP_VERTS), MAP_VERTS);
            staticLineRenderer.vertCount = (GLsizei)(sizeof(MAP_VERTS) / (sizeof(float) * 6));
            registry.assign<StaticLinesRenderer>(entity, staticLineRenderer);
        }

        // Hud elements
        {
            auto entity = registry.create();
            registry.assign<Label::HUD>(entt::tag_t{}, entity);

            auto &hud = registry.assign<Hud>(entity);
            hud.currentWaveLabel = UI::createLabel(registry, "", { 0, 0 }, Color{ 0.75f, 0.75f, 0.75f, 1 });
            hud.cashLabel = UI::createLabel(registry, "", { 16, 0 }, Color{ 1, 1, 0, 1 }, 1.0f);

            UI::createTowerButton(registry, "Gun Turret", { -1, 16.5f }, GUN_TOWER_LVL1_PRICE,
                [](Registry &registry, Entity button) { Tower::createPlacer(registry, Tower::buyGunTurret); });

            UI::createTowerButton(registry, "Slow Turret", { 5, 16.5f }, SLOW_TOWER_LVL1_PRICE,
                [](Registry &registry, Entity button) { Tower::createPlacer(registry, Tower::buySlowTurret); });

            UI::createTowerButton(registry, "Rocket Turret", { 11, 16.5f }, ROCKET_TOWER_LVL1_PRICE,
                [](Registry &registry, Entity button) { Tower::createPlacer(registry, Tower::buyRocketTurret); });

            UI::createTowerButton(registry, "Cannon Turret", { 17, 16.5f }, CANNON_TOWER_LVL1_PRICE,
                [](Registry &registry, Entity button) { Tower::createPlacer(registry, Tower::buyCannonTurret); });
        }

        // Wave manager then start the first wave
        {
            Waves::createWavesController(registry);
            Waves::startWave(registry, 0);
        }

        // Player
        {
            auto entity = registry.create();
            registry.assign<Label::PLAYER>(entt::tag_t{}, entity);
            registry.assign<Bank>(entity, 150.0f);
            registry.assign<Investor>(entity, 0.003f);
        }

        // Tool tip
        {
            auto entity = registry.create();
            registry.assign<Label::TOOLTIP>(entt::tag_t{}, entity);
        }

        // UI Context
        {
            auto entity = registry.create();
            registry.assign<Label::UI_CONTEXT>(entt::tag_t{}, entity);
            registry.assign<UIContext>(entity, NULL_ENTITY, NULL_ENTITY);
        }

        // Inputs
        {
            auto entity = registry.create();
            registry.assign<Label::INPUTS>(entt::tag_t{}, entity);

            Inputs inputs;
            memset(&inputs, 0, sizeof(Inputs));
            registry.assign<Inputs>(entity, inputs);
        }
    }

    void updateSim(Registry &registry, float dt)
    {
        updateSpawnSystem(registry, dt);
        updateEndWaveCheckSystem(registry);
        updateEndWaveDelaySystem(registry, dt);
        updateTravelSystem(registry, dt);
        updateInvestmentSystem(registry, dt);
        updateCooldownSystem(registry, dt);
        updateTargetingSystem(registry, dt);
        updateDeathTimerSystem(registry, dt);
        updateColorAnimSystem(registry, dt);
        updateLineAnimSystem(registry, dt);
        updateSizePulseAnimSystem(registry, dt);
        updatePhysicSystem(registry, dt);
        updateKillSystem(registry);
    }

    void update(Registry &registry, float dt)
    {
        updateInputSystem(registry);
        updateHudSystem(registry);
        updateCashThresholdButtonToggleSystem(registry);
        updateHoverSystem(registry);
        updateClickSystem(registry);
        updateUpgradeSystem(registry);
        updateToolTipSystem(registry);
        updateButtonStateSystem(registry);
        updatePlaceSystem(registry);
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
            drawDebugView(registry);
        }

        Rendering::endFrame();
    }
};
