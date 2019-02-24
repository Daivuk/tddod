#include "TargetingSystem.h"

#include "components/Cooldown.h"
#include "components/FindTarget.h"
#include "components/Position.h"
#include "components/Target.h"
#include "components/Targeter.h"

void updateTargetingSystem(Registry &registry, float dt)
{
    registry.view<Targeter, FindTarget, Position>().each([&registry, dt](auto entity, const Targeter &targeter, const FindTarget &findTarget, const Position& position)
    {
        auto rangeSqr = targeter.range * targeter.range;
        auto targetEntity = NULL_ENTITY;
        registry.view<Target, Position>().each([dt, &rangeSqr, &position, &targetEntity, &targeter](auto entity, const Target& target, const Position& critterPosition)
        {
            if (target.mask & targeter.targetMask)
            {
                auto dx = critterPosition.x - position.x;
                auto dy = critterPosition.y - position.y;
                auto distSqr = dx * dx + dy * dy;
                if (distSqr <= rangeSqr)
                {
                    targetEntity = entity;
                    rangeSqr = distSqr; // Reduce range, we will always target the closest (That could be very bad design for a TD... oh well)
                }
            }
        });

        if (targetEntity != NULL_ENTITY)
        {
            // Shoot at target
            registry.remove<FindTarget>(entity);
            registry.assign<Cooldown>(entity, targeter.cooldown);
            targeter.shoot(registry, targetEntity, entity);
        }
    });
}
