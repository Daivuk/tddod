#include "components/BaseSpeed.h"
#include "components/Speed.h"
#include "systems/ResetSpeedSystem.h"

void updateResetSpeedSystem(Registry &registry)
{
    registry.view<BaseSpeed, Speed>().each([](auto entity, const BaseSpeed &baseSpeed, Speed &speed)
    {
        speed.speed = baseSpeed.speed;
    });
}
