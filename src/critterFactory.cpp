#include <entt/entt.hpp>

#include "critterFactory.h"
#include "components.h"
#include "data.h"

extern Registry g_registry;

void createPeasant(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.15f;
    auto speed = 45.0f / 60.0f;

    g_registry.assign<Critter>(entity);
    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, Color{ 251.0f / 255.0f, 242.0f / 255.0f, 54.0f / 255.0f, 1.0f });
    g_registry.assign<ShapeRenderer>(entity, drawCircle);
    g_registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
    g_registry.assign<Name>(entity, "Peasant");
    g_registry.assign<Health>(entity, 30.0f, 30.0f);
    g_registry.assign<Bank>(entity, 10.0f);
    g_registry.assign<Target>(entity, TargetMask::GROUND);
}

void createWarrior(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.20f;
    auto speed = 45.0f / 60.0f;

    g_registry.assign<Critter>(entity);
    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, Color{ 223.0f / 255.0f, 113.0f / 255.0f, 38.0f / 255.0f, 1.0f });
    g_registry.assign<ShapeRenderer>(entity, drawCircle);
    g_registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
    g_registry.assign<Name>(entity, "Warrior");
    g_registry.assign<Health>(entity, 50.0f, 50.0f);
    g_registry.assign<Bank>(entity, 10.0f);
    g_registry.assign<Target>(entity, TargetMask::GROUND);
}

void createThief(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.10f;
    auto speed = 45.0f / 60.0f * 4.0f;

    g_registry.assign<Critter>(entity);
    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, Color{ 0.5f, 0.75f, 1.0f, 1.0f });
    g_registry.assign<ShapeRenderer>(entity, drawCircle);
    g_registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
    g_registry.assign<Name>(entity, "Thief");
    g_registry.assign<Health>(entity, 30.0f, 30.0f);
    g_registry.assign<Bank>(entity, 5.0f);
    g_registry.assign<Target>(entity, TargetMask::GROUND);
}

void createFlyer(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.20f;
    auto speed = 45.0f / 60.0f * 2.0f;

    g_registry.assign<Critter>(entity);
    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
    g_registry.assign<ShapeRenderer>(entity, drawCircle);
    g_registry.assign<Traveler>(entity, speed, 0, AIR_WAYPOINT_COUNT, (Position*)AIR_WAYPOINTS);
    g_registry.assign<Name>(entity, "Flyer");
    g_registry.assign<Health>(entity, 30.0f, 30.0f);
    g_registry.assign<Bank>(entity, 10.0f);
    g_registry.assign<Target>(entity, TargetMask::AIR);
}

void createBehemoth(const Position& position)
{
    auto entity = g_registry.create();

    auto size = 0.40f;
    auto speed = 45.0f / 60.0f / 2.0f;

    g_registry.assign<Critter>(entity);
    g_registry.assign<Position>(entity, position);
    g_registry.assign<Size>(entity, size, size);
    g_registry.assign<Color>(entity, Color{ 1.0f, 0.25f, 0.25f, 1.0f });
    g_registry.assign<ShapeRenderer>(entity, drawCircle);
    g_registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
    g_registry.assign<Name>(entity, "Behemoth");
    g_registry.assign<Health>(entity, 600.0f, 600.0f);
    g_registry.assign<Bank>(entity, 50.0f);
    g_registry.assign<Target>(entity, TargetMask::GROUND);
}
