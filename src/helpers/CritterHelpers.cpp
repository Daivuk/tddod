#include "data.h"

#include "components/Bank.h"
#include "components/Color.h"
#include "components/Critter.h"
#include "components/Health.h"
#include "components/Hoverable.h"
#include "components/Name.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"
#include "components/Target.h"
#include "components/Traveler.h"
#include "helpers/CritterHelpers.h"
#include "helpers/ShapeHelpers.h"

namespace Critters
{
    void createPeasant(Registry &registry, const Position& position)
    {
        auto entity = registry.create();

        auto size = 0.15f;
        auto speed = 45.0f / 60.0f;

        registry.assign<Critter>(entity);
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, size, size);
        registry.assign<Color>(entity, Color{ 251.0f / 255.0f, 242.0f / 255.0f, 54.0f / 255.0f, 1.0f });
        registry.assign<ShapeRenderer>(entity, Shape::drawCircle);
        registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
        registry.assign<Name>(entity, "Peasant");
        registry.assign<Health>(entity, 30.0f, 30.0f);
        registry.assign<Bank>(entity, 10.0f);
        registry.assign<Target>(entity, TargetMask::GROUND);
        registry.assign<Hoverable>(entity);
    }

    void createWarrior(Registry &registry, const Position& position)
    {
        auto entity = registry.create();

        auto size = 0.20f;
        auto speed = 45.0f / 60.0f;

        registry.assign<Critter>(entity);
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, size, size);
        registry.assign<Color>(entity, Color{ 223.0f / 255.0f, 113.0f / 255.0f, 38.0f / 255.0f, 1.0f });
        registry.assign<ShapeRenderer>(entity, Shape::drawCircle);
        registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
        registry.assign<Name>(entity, "Warrior");
        registry.assign<Health>(entity, 50.0f, 50.0f);
        registry.assign<Bank>(entity, 10.0f);
        registry.assign<Target>(entity, TargetMask::GROUND);
        registry.assign<Hoverable>(entity);
    }

    void createThief(Registry &registry, const Position& position)
    {
        auto entity = registry.create();

        auto size = 0.10f;
        auto speed = 45.0f / 60.0f * 4.0f;

        registry.assign<Critter>(entity);
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, size, size);
        registry.assign<Color>(entity, Color{ 0.5f, 0.75f, 1.0f, 1.0f });
        registry.assign<ShapeRenderer>(entity, Shape::drawCircle);
        registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
        registry.assign<Name>(entity, "Thief");
        registry.assign<Health>(entity, 30.0f, 30.0f);
        registry.assign<Bank>(entity, 5.0f);
        registry.assign<Target>(entity, TargetMask::GROUND);
        registry.assign<Hoverable>(entity);
    }

    void createFlyer(Registry &registry, const Position& position)
    {
        auto entity = registry.create();

        auto size = 0.20f;
        auto speed = 45.0f / 60.0f * 2.0f;

        registry.assign<Critter>(entity);
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, size, size);
        registry.assign<Color>(entity, Color{ 1.0f, 0.0f, 1.0f, 1.0f });
        registry.assign<ShapeRenderer>(entity, Shape::drawCircle);
        registry.assign<Traveler>(entity, speed, 0, AIR_WAYPOINT_COUNT, (Position*)AIR_WAYPOINTS);
        registry.assign<Name>(entity, "Flyer");
        registry.assign<Health>(entity, 30.0f, 30.0f);
        registry.assign<Bank>(entity, 10.0f);
        registry.assign<Target>(entity, TargetMask::AIR);
        registry.assign<Hoverable>(entity);
    }

    void createBehemoth(Registry &registry, const Position& position)
    {
        auto entity = registry.create();

        auto size = 0.40f;
        auto speed = 45.0f / 60.0f / 2.0f;

        registry.assign<Critter>(entity);
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, size, size);
        registry.assign<Color>(entity, Color{ 1.0f, 0.25f, 0.25f, 1.0f });
        registry.assign<ShapeRenderer>(entity, Shape::drawCircle);
        registry.assign<Traveler>(entity, speed, 0, WAYPOINT_COUNT, (Position*)WAYPOINTS);
        registry.assign<Name>(entity, "Behemoth");
        registry.assign<Health>(entity, 600.0f, 600.0f);
        registry.assign<Bank>(entity, 50.0f);
        registry.assign<Target>(entity, TargetMask::GROUND);
        registry.assign<Hoverable>(entity);
    }
};
