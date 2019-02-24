#include <entt/entt.hpp>

#include "turretFactory.h"
#include "components.h"
#include "data.h"

extern Registry g_registry;

static const Color LEVEL1_COLOR = { 0.15f, 1.0f, 0.15f, 1.0f };
static const Color LEVEL2_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
static const Color LEVEL3_COLOR = { 1.0f, 0.35f, 0.35f, 1.0f };

void spawnParticlesFromShape(const Position& position, const ShapeRenderer& shapeRenderer, const Size& size, const Color& color);

static void createBullet(const Position& from, const Position& to, const Color& color)
{
    // Left vector
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float l = std::sqrtf(dx * dx + dy * dy);
    dx /= l;
    dy /= l;
    float lx = -dy * 0.25f;
    float ly = dx * 0.25f;

    // Left line
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, from);
        g_registry.assign<LineRenderer>(entity, to);
        g_registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
        g_registry.assign<ColorAnim>(entity, 0.0f, 0.5f, Color{ 0, 0.8f, 1, 0.5f }, Color{ 0, 0, 0, 0 });
        g_registry.assign<DeathTimer>(entity, 0.5f);
        g_registry.assign<LineAnim>(entity, 0.0f, 0.5f, from, to,
            Position{ from.x - lx, from.y - ly },
            Position{ to.x - lx, to.y - ly });
    }

    // Right line
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, from);
        g_registry.assign<LineRenderer>(entity, to);
        g_registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
        g_registry.assign<ColorAnim>(entity, 0.0f, 0.5f, Color{ 0, 0.8f, 1, 0.5f }, Color{ 0, 0, 0, 0 });
        g_registry.assign<DeathTimer>(entity, 0.5f);
        g_registry.assign<LineAnim>(entity, 0.0f, 0.5f, from, to,
            Position{ from.x + lx, from.y + ly },
            Position{ to.x + lx, to.y + ly });
    }

    // Center line
    {
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, from);
        g_registry.assign<LineRenderer>(entity, to);
        g_registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
        g_registry.assign<ColorAnim>(entity, 0.0f, 0.25f, Color{ color.r, color.g, color.b, color.a * 0.75f }, Color{ 0, 0, 0, 0 });
        g_registry.assign<DeathTimer>(entity, 0.25f);
    }

    // Bullet
    {
        const auto bulletSize = 0.5f;
        auto entity = g_registry.create();
        g_registry.assign<Position>(entity, from);
        g_registry.assign<LineRenderer>(entity, to);
        g_registry.assign<Color>(entity, color);
        g_registry.assign<DeathTimer>(entity, l / 30.0f);
        g_registry.assign<LineAnim>(entity, 0.0f, l / 30.0f,
            Position{ from.x, from.y },
            Position{ from.x + dx * bulletSize, from.y + dy * bulletSize },
            Position{ to.x - dx * bulletSize, to.y - dy * bulletSize },
            Position{ to.x, to.y });
    }
}

static void kill(Entity target)
{
    if (g_registry.has<Dead>(target)) return; // Don't kill twice
    if (g_registry.has<Bank>(target))
    {
        auto amount = g_registry.get<Bank>(target).amount;
        auto view = g_registry.view<Bank, Player>();
        if (!view.empty()) amount /= (float)view.size();
        view.each([amount](auto entity, Bank& bank, const auto& player)
        {
            bank.amount += amount;
        });
    }
    if (g_registry.has<Position, ShapeRenderer, Size, Color>(target))
    {
        spawnParticlesFromShape(g_registry.get<Position>(target), g_registry.get<ShapeRenderer>(target), g_registry.get<Size>(target), g_registry.get<Color>(target));
    }
    g_registry.accommodate<Dead>(target);
}

static void damage(Entity target, float damage)
{
    if (g_registry.has<Health>(target))
    {
        auto& health = g_registry.get<Health>(target);
        health.amount -= damage;
        if (health.amount <= 0.0f)
        {
            kill(target);
        }
    }
}

static void shootGunLvl1(Entity target, Entity from)
{
    // Instant ray. Spawn fx
    createBullet(g_registry.get<Position>(from), g_registry.get<Position>(target), { 0, 1, 0, 1 });
    damage(target, 10.0f);
}

static void shootGunLvl2(Entity target, Entity from)
{
    // Instant ray. Spawn fx
    createBullet(g_registry.get<Position>(from), g_registry.get<Position>(target), { 1, 1, 0, 1 });
    damage(target, 15.0f);
}

static void shootGunLvl3(Entity target, Entity from)
{
    // Instant ray. Spawn fx
    createBullet(g_registry.get<Position>(from), g_registry.get<Position>(target), { 1, 0.75f, 0.75f, 1 });
    damage(target, 17.5f);
}

static void upgradeGunLvl3(Entity entity)
{
    g_registry.accommodate<Color>(entity, LEVEL3_COLOR);
    g_registry.accommodate<Name>(entity, "Gun Turret 3");
    g_registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f + 1.0f, 0.25f, shootGunLvl3);
    g_registry.reset<Upgradable>(entity);
}

static void upgradeGunLvl2(Entity entity)
{
    g_registry.accommodate<Color>(entity, LEVEL2_COLOR);
    g_registry.accommodate<Name>(entity, "Gun Turret 2 - \"U\" upgrade $400");
    g_registry.accommodate<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f + 0.5f, 0.5f, shootGunLvl2);
    g_registry.accommodate<Upgradable>(entity, 400.0f, upgradeGunLvl3);
}

static void shootSlowLvl1(Entity target, Entity from)
{

}

static void shootRocketLvl1(Entity target, Entity from)
{

}

static void shootCannonLvl1(Entity target, Entity from)
{

}

static void charge(float cost)
{
    g_registry.view<Player, Bank>().each([cost](auto entity, const Player& player, Bank& bank)
    {
        bank.amount -= cost;
    });
}

void createGunTurret(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.4f;

    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, LEVEL1_COLOR);
    g_registry.assign<TextRenderer>(entity, "G", Color{ 0, 0, 0, 0 }, 0.5f);
    g_registry.assign<ShapeRenderer>(entity, drawBox);
    g_registry.assign<Name>(entity, "Gun Turret 1 - \"U\" upgrade $200");
    g_registry.assign<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f, 1.0f, shootGunLvl1);
    g_registry.assign<Upgradable>(entity, 200.0f, upgradeGunLvl2);
    g_registry.assign<FindTarget>(entity);

    charge(100.0f);
}

void createSlowTurret(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.4f;

    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, LEVEL1_COLOR);
    g_registry.assign<TextRenderer>(entity, "S", Color{ 0, 0, 0, 0 }, 0.5f);
    g_registry.assign<ShapeRenderer>(entity, drawBox);
    g_registry.assign<Name>(entity, "Slow Turret");
    g_registry.assign<Targeter>(entity, (uint8_t)(TargetMask::GROUND | TargetMask::AIR), 210.0f / 60.0f, 1.0f, shootSlowLvl1);
    g_registry.assign<FindTarget>(entity);

    charge(200.0f);
}

void createRocketTurret(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.4f;

    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, LEVEL1_COLOR);
    g_registry.assign<TextRenderer>(entity, "R", Color{ 0, 0, 0, 0 }, 0.5f);
    g_registry.assign<ShapeRenderer>(entity, drawBox);
    g_registry.assign<Name>(entity, "Rocket Turret");
    g_registry.assign<Targeter>(entity, TargetMask::AIR, 210.0f / 60.0f, 1.0f, shootRocketLvl1);
    g_registry.assign<FindTarget>(entity);

    charge(300.0f);
}

void createCannonTurret(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.4f;

    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, LEVEL1_COLOR);
    g_registry.assign<TextRenderer>(entity, "C", Color{ 0, 0, 0, 0 }, 0.5f);
    g_registry.assign<ShapeRenderer>(entity, drawBox);
    g_registry.assign<Name>(entity, "Cannon Turret");
    g_registry.assign<Targeter>(entity, TargetMask::GROUND, 210.0f / 60.0f, 1.0f, shootCannonLvl1);
    g_registry.assign<FindTarget>(entity);

    charge(400.0f);
}
