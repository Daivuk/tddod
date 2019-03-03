#include "constants.h"
#include "data.h"
#include "tags.h"

#include "components/Color.h"
#include "components/FindTarget.h"
#include "components/Hoverable.h"
#include "components/Name.h"
#include "components/Resources.h"
#include "components/Size.h"
#include "components/SizePulseAnim.h"
#include "components/ShapeRenderer.h"
#include "components/Target.h"
#include "components/Targeter.h"
#include "components/TextRenderer.h"
#include "components/Upgradable.h"
#include "helpers/AudioHelpers.h"
#include "helpers/BankHelpers.h"
#include "helpers/ShapeHelpers.h"
#include "helpers/ShootingHelpers.h"
#include "helpers/TowerHelpers.h"

static void shootGunLvl1(Registry &registry, Entity target, Entity from)
{
    // Instant ray. Spawn fx
    Shooting::createBullet(registry, registry.get<Position>(from), registry.get<Position>(target), { 0, 1, 0, 1 });
    Shooting::damage(registry, target, 10.0f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.gunSound);
}

static void shootGunLvl2(Registry &registry, Entity target, Entity from)
{
    // Instant ray. Spawn fx
    Shooting::createBullet(registry, registry.get<Position>(from), registry.get<Position>(target), { 1, 1, 0, 1 });
    Shooting::damage(registry, target, 15.0f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.gunSound);
}

static void shootGunLvl3(Registry &registry, Entity target, Entity from)
{
    // Instant ray. Spawn fx
    Shooting::createBullet(registry, registry.get<Position>(from), registry.get<Position>(target), { 1, 0.75f, 0.75f, 1 });
    Shooting::damage(registry, target, 17.5f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.gunSound);
}

static void shootSlowLvl1(Registry &registry, Entity target, Entity from)
{
    Shooting::createSlowBolt(registry, registry.get<Position>(from), registry.get<Position>(target), TOWER_LEVEL1_COLOR);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.slowSound);
}

static void shootRocketLvl1(Registry &registry, Entity target, Entity from)
{

}

static void shootCannonLvl1(Registry &registry, Entity target, Entity from)
{
    Shooting::createCannonBall(registry, registry.get<Position>(from), registry.get<Position>(target), TOWER_LEVEL1_COLOR, 5.0f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.cannonSound);
}

static void shootCannonLvl2(Registry &registry, Entity target, Entity from)
{
    Shooting::createCannonBall(registry, registry.get<Position>(from), registry.get<Position>(target), TOWER_LEVEL2_COLOR, 15.0f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.cannonSound);
}

static void shootCannonLvl3(Registry &registry, Entity target, Entity from)
{
    Shooting::createCannonBall(registry, registry.get<Position>(from), registry.get<Position>(target), TOWER_LEVEL3_COLOR, 27.0f);

    const auto &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());
    Audio::playSound(registry, resources.cannonSound);
}

static void upgradeGunLvl3(Registry &registry, Entity entity)
{
    registry.accommodate<Color>(entity, TOWER_LEVEL3_COLOR);
    registry.accommodate<Name>(entity, "Gun Turret 3");
    registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f + 1.0f, 0.25f, shootGunLvl3);
    registry.reset<Upgradable>(entity);
}

static void upgradeGunLvl2(Registry &registry, Entity entity)
{
    registry.accommodate<Color>(entity, TOWER_LEVEL2_COLOR);
    registry.accommodate<Name>(entity, "Gun Turret 2 - \"U\" upgrade $400");
    registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f + 0.5f, 0.5f, shootGunLvl2);
    registry.accommodate<Upgradable>(entity, (float)GUN_TOWER_LVL3_PRICE, upgradeGunLvl3);
}

static void upgradeCannonLvl3(Registry &registry, Entity entity)
{
    registry.accommodate<Color>(entity, TOWER_LEVEL3_COLOR);
    registry.accommodate<Name>(entity, "Cannon Turret 3");
    registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND), 210.0f / 60.0f + 1.0f, 0.75f, shootCannonLvl3);
    registry.reset<Upgradable>(entity);
}

static void upgradeCannonLvl2(Registry &registry, Entity entity)
{
    registry.accommodate<Color>(entity, TOWER_LEVEL2_COLOR);
    registry.accommodate<Name>(entity, "Cannon Turret 2 - \"U\" upgrade $1000");
    registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND), 210.0f / 60.0f + 0.5f, 1.0f, shootCannonLvl2);
    registry.accommodate<Upgradable>(entity, (float)GUN_TOWER_LVL3_PRICE, upgradeCannonLvl3);
}

namespace Tower
{
    void createPlacer(Registry &registry, PLACE_FN place)
    {
        // Clear previous, we only accept 1 placer
        registry.destroy<Placer>();

        auto entity = registry.create();
        registry.assign<Placer>(entity, place);
        registry.assign<Color>(entity, Color{ 0, 1, 1, 0.75f });
        registry.assign<Size>(entity, Size{ 0.45f, 0.45f });
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<SizePulseAnim>(entity, 0.0f, 20.0f, 0.35f, 0.45f);
    }

    void buyGunTurret(Registry &registry, const Position &position)
    {
        if (!Money::withdraw(registry, registry.attachee<Tag::Player>(), GUN_TOWER_LVL1_PRICE)) return;

        auto entity = registry.create();

        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, TOWER_SIZE, TOWER_SIZE);
        registry.assign<Color>(entity, TOWER_LEVEL1_COLOR);
        registry.assign<TextRenderer>(entity, "G", Color{ 0, 0, 0, 0 }, 0.5f);
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<Name>(entity, "Gun Turret 1 - \"U\" upgrade $200");
        registry.assign<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f, 1.0f, shootGunLvl1);
        registry.assign<Upgradable>(entity, (float)GUN_TOWER_LVL2_PRICE, upgradeGunLvl2);
        registry.assign<Hoverable>(entity);
        registry.assign<FindTarget>(entity);
    }

    void buySlowTurret(Registry &registry, const Position &position)
    {
        if (!Money::withdraw(registry, registry.attachee<Tag::Player>(), SLOW_TOWER_LVL1_PRICE)) return;

        auto entity = registry.create();

        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, TOWER_SIZE, TOWER_SIZE);
        registry.assign<Color>(entity, TOWER_LEVEL1_COLOR);
        registry.assign<TextRenderer>(entity, "S", Color{ 0, 0, 0, 0 }, 0.5f);
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<Name>(entity, "Slow Turret");
        registry.assign<Targeter>(entity, (uint8_t)(TargetMask::GROUND), 210.0f / 60.0f, 2.5f, shootSlowLvl1);
        registry.assign<FindTarget>(entity);
        registry.assign<Hoverable>(entity);
    }

    void buyRocketTurret(Registry &registry, const Position &position)
    {
        if (!Money::withdraw(registry, registry.attachee<Tag::Player>(),  ROCKET_TOWER_LVL1_PRICE)) return;

        auto entity = registry.create();

        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, TOWER_SIZE, TOWER_SIZE);
        registry.assign<Color>(entity, TOWER_LEVEL1_COLOR);
        registry.assign<TextRenderer>(entity, "R", Color{ 0, 0, 0, 0 }, 0.5f);
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<Name>(entity, "Rocket Turret");
        registry.assign<Targeter>(entity, TargetMask::AIR, 210.0f / 60.0f, 1.0f, shootRocketLvl1);
        registry.assign<FindTarget>(entity);
        registry.assign<Hoverable>(entity);
    }

    void buyCannonTurret(Registry &registry, const Position &position)
    {
        if (!Money::withdraw(registry, registry.attachee<Tag::Player>(), CANNON_TOWER_LVL1_PRICE)) return;

        auto entity = registry.create();

        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, TOWER_SIZE, TOWER_SIZE);
        registry.assign<Color>(entity, TOWER_LEVEL1_COLOR);
        registry.assign<TextRenderer>(entity, "C", Color{ 0, 0, 0, 0 }, 0.5f);
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<Name>(entity, "Cannon Turret - \"U\" upgrade $500");
        registry.assign<Targeter>(entity, TargetMask::GROUND, 210.0f / 60.0f, 1.0f, shootCannonLvl1);
        registry.assign<FindTarget>(entity);
        registry.assign<Hoverable>(entity);
        registry.assign<Upgradable>(entity, (float)CANNON_TOWER_LVL2_PRICE, upgradeCannonLvl2);
    }
}
