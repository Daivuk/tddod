#include "DeathTimerSystem.h"

#include "components/DeathTimer.h"

void updateDeathTimerSystem(Registry &registry, float dt)
{
    registry.view<DeathTimer>().each([&registry, dt](auto entity, DeathTimer& deathTimer)
    {
        auto delay = deathTimer.delay - dt;
        if (delay <= 0.0f)
        {
            registry.destroy(entity);
            return;
        }
        deathTimer.delay = delay;
    });
}
