/*
    Main loop calls into this namespace.
    The Game sould probably be an object that hold his own Registry, but
    for simplicity, we only use 1 global Registry declared in main.
*/

#include "constants.h"
#include "data.h"
#include "game.h"
#include "tags.h"

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
#include "systems/AudioMixerSystem.h"
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
#include "systems/PositionAnimSystem.h"
#include "systems/ResetSpeedSystem.h"
#include "systems/ShapeRenderSystem.h"
#include "systems/SizePulseAnimSystem.h"
#include "systems/SpawnSystem.h"
#include "systems/SpeedNerfSystem.h"
#include "systems/StaticLinesRenderSystem.h"
#include "systems/TargetingSystem.h"
#include "systems/TextRenderSystem.h"
#include "systems/ToolTipSystem.h"
#include "systems/TravelerTargetScoreSystem.h"
#include "systems/TravelSystem.h"
#include "systems/UpgradeSystem.h"

// Helpers
#include "helpers/AudioHelpers.h"
#include "helpers/RenderingHelpers.h"
#include "helpers/TowerHelpers.h"
#include "helpers/UIHelpers.h"
#include "helpers/WaveHelpers.h"

namespace Game
{
    void init(Registry &registry)
    {
        // Load resources
        {
            auto entity = registry.create();
            registry.assign<Tag::Resources>(entt::tag_t{}, entity);

            Resources resources;
            resources.programPC = Rendering::createProgram(PC_VERT, PC_FRAG, {"Position", "Color"});
            resources.programPTC = Rendering::createProgram(PTC_VERT, PTC_FRAG, {"Position", "TexCoord", "Color"});
            resources.fontTexture = Rendering::createTexture("assets/font.png");
            resources.vertexBuffer = Rendering::createVertexBuffer();
            resources.pPCVertices = new VertexPC[MAX_VERTICES];
            resources.pPTCVertices = new VertexPTC[MAX_VERTICES];
            resources.gunSound = Audio::loadSound("assets/gun.wav");
            resources.popSound = Audio::loadSound("assets/pop.wav");
            resources.slowSound = Audio::loadSound("assets/slow.wav");
            resources.cannonSound = Audio::loadSound("assets/cannon.wav");
            resources.explosionSound = Audio::loadSound("assets/explosion.wav");
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
            registry.assign<Tag::Hud>(entt::tag_t{}, entity);

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
            registry.assign<Tag::Player>(entt::tag_t{}, entity);
            registry.assign<Bank>(entity, 150.0f);
            registry.assign<Investor>(entity, 0.003f);
        }

        // Tool tip
        {
            auto entity = registry.create();
            registry.assign<Tag::Tooltip>(entt::tag_t{}, entity);
        }

        // UI Context
        {
            auto entity = registry.create();
            registry.assign<Tag::UIContext>(entt::tag_t{}, entity);
            registry.assign<UIContext>(entity, NULL_ENTITY, NULL_ENTITY);
        }

        // Inputs
        {
            auto entity = registry.create();
            registry.assign<Tag::Inputs>(entt::tag_t{}, entity);

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
        updateResetSpeedSystem(registry);
        updateSpeedNerfSystem(registry, dt);
        updateTravelSystem(registry, dt);
        updateTravelerTargetScoreSystem(registry);
        updateInvestmentSystem(registry, dt);
        updateCooldownSystem(registry, dt);
        updateTargetingSystem(registry, dt);
        updateDeathTimerSystem(registry, dt);
        updateColorAnimSystem(registry, dt);
        updateLineAnimSystem(registry, dt);
        updatePositionAnimSystem(registry, dt);
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

    void updateAudio(Registry &registry, uint8_t *buffer, int len)
    {
        updateAudioMixerSystem(registry, buffer, len);
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

        Rendering::endFrame();
    }
};
