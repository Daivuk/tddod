#include "components/Speed.h"
#include "components/SpeedNerf.h"
#include "systems/SpeedNerfSystem.h"

void updateSpeedNerfSystem(Registry &registry, float dt)
{
    registry.view<SpeedNerf, Speed>().each([&registry, dt](auto entity, SpeedNerf &speedNerf, Speed &speed)
    {
        auto delay = speedNerf.delay - dt;
        speed.speed *= speedNerf.speedMult;
        if (delay <= 0.0f)
        {
            registry.remove<SpeedNerf>(entity);
            return;
        }
        speedNerf.delay = delay;
    });
}
