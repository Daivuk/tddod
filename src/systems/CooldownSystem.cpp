#include "CooldownSystem.h"

#include "components/Cooldown.h"
#include "components/FindTarget.h"

void updateCooldownSystem(Registry &registry, float dt)
{
    registry.view<Cooldown>().each([&registry, dt](auto entity, Cooldown &cooldown)
    {
        auto delay = cooldown.delay - dt;
        if (delay <= 0.0f)
        {
            registry.remove<Cooldown>(entity);
            registry.assign<FindTarget>(entity);
            return;
        }
        cooldown.delay = delay;
    });
}
