#include "labels.h"

#include "components/Color.h"
#include "components/ColorAnim.h"
#include "components/Dead.h"
#include "components/DeathTimer.h"
#include "components/Health.h"
#include "components/LineAnim.h"
#include "components/LineRenderer.h"
#include "components/Position.h"
#include "components/ShapeRenderer.h"
#include "components/SpeedNerf.h"
#include "components/Target.h"
#include "helpers/BankHelpers.h"
#include "helpers/ParticleHelpers.h"
#include "helpers/ShootingHelpers.h"

namespace Shooting
{
    void createSlowBolt(Registry &registry, const Position& from, const Position& to, const Color& color)
    {

    }

    void createBullet(Registry &registry, const Position& from, const Position& to, const Color& color)
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
            auto entity = registry.create();
            registry.assign<Position>(entity, from);
            registry.assign<LineRenderer>(entity, to);
            registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
            registry.assign<ColorAnim>(entity, 0.0f, 0.5f, Color{ 0, 0.8f, 1, 0.5f }, Color{ 0, 0, 0, 0 });
            registry.assign<DeathTimer>(entity, 0.5f);
            registry.assign<LineAnim>(entity, 0.0f, 0.5f, from, to,
                Position{ from.x - lx, from.y - ly },
                Position{ to.x - lx, to.y - ly });
        }

        // Right line
        {
            auto entity = registry.create();
            registry.assign<Position>(entity, from);
            registry.assign<LineRenderer>(entity, to);
            registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
            registry.assign<ColorAnim>(entity, 0.0f, 0.5f, Color{ 0, 0.8f, 1, 0.5f }, Color{ 0, 0, 0, 0 });
            registry.assign<DeathTimer>(entity, 0.5f);
            registry.assign<LineAnim>(entity, 0.0f, 0.5f, from, to,
                Position{ from.x + lx, from.y + ly },
                Position{ to.x + lx, to.y + ly });
        }

        // Center line
        {
            auto entity = registry.create();
            registry.assign<Position>(entity, from);
            registry.assign<LineRenderer>(entity, to);
            registry.assign<Color>(entity, Color{ 1, 1, 1, 1 });
            registry.assign<ColorAnim>(entity, 0.0f, 0.25f, Color{ color.r, color.g, color.b, color.a * 0.75f }, Color{ 0, 0, 0, 0 });
            registry.assign<DeathTimer>(entity, 0.25f);
        }

        // Bullet
        {
            const auto bulletSize = 0.5f;
            auto entity = registry.create();
            registry.assign<Position>(entity, from);
            registry.assign<LineRenderer>(entity, to);
            registry.assign<Color>(entity, color);
            registry.assign<DeathTimer>(entity, l / 30.0f);
            registry.assign<LineAnim>(entity, 0.0f, l / 30.0f,
                Position{ from.x, from.y },
                Position{ from.x + dx * bulletSize, from.y + dy * bulletSize },
                Position{ to.x - dx * bulletSize, to.y - dy * bulletSize },
                Position{ to.x, to.y });
        }
    }

    void kill(Registry &registry, Entity target)
    {
        if (registry.has<Dead>(target)) return; // Don't kill twice
        Money::transfer(registry, target, registry.attachee<Label::PLAYER>());
        if (registry.has<Position, ShapeRenderer, Size, Color>(target))
        {
            PFX::spawnParticlesFromShape(registry, 
                registry.get<Position>(target), 
                registry.get<ShapeRenderer>(target),
                registry.get<Size>(target), 
                registry.get<Color>(target));
        }
        registry.accommodate<Dead>(target);
    }

    void damage(Registry &registry, Entity target, float damage)
    {
        if (registry.has<Health>(target))
        {
            auto& health = registry.get<Health>(target);
            health.amount -= damage;
            if (health.amount <= 0.0f)
            {
                kill(registry, target);
            }
        }
    }

    void slow(Registry &registry, const Position &hitPosition, float radius, float speedMult)
    {
        auto rangeSqr = radius * radius;
        registry.view<Target, Position>().each([&registry, &hitPosition, radius, speedMult, rangeSqr](auto entity, const Target &target, const Position &position)
        {
            if (target.mask & TargetMask::GROUND) // Only ground are affected
            {
                auto dx = position.x - hitPosition.x;
                auto dy = position.y - hitPosition.y;
                auto distSqr = dx * dx + dy * dy;
                if (distSqr <= rangeSqr)
                {
                    registry.accommodate<SpeedNerf>(entity, 2.333f, speedMult);
                }
            }
        });
    }
};
