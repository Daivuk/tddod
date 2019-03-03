#include "components/Position.h"
#include "components/PositionAnim.h"
#include "components/PositionAnimCallback.h"
#include "systems/PositionAnimSystem.h"

void updatePositionAnimSystem(Registry &registry, float dt)
{
    registry.view<PositionAnim, Position>().each([&registry, dt](auto entity, PositionAnim &positionAnim, Position &position)
    {
        auto anim = positionAnim.anim + dt;
        auto t = std::min(anim, positionAnim.duration) / positionAnim.duration;
        position.x = positionAnim.from.x + (positionAnim.to.x - positionAnim.from.x) * t;
        position.y = positionAnim.from.y + (positionAnim.to.y - positionAnim.from.y) * t;
        positionAnim.anim = anim;

        if (anim >= positionAnim.duration)
        {
            if (registry.has<PositionAnimCallback>(entity))
            {
                registry.get<PositionAnimCallback>(entity).callback(registry, entity);
            }
        }
    });
}
