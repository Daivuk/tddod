#include "PhysicSystem.h"

#include "components/Physics.h"
#include "components/Position.h"

void updatePhysicSystem(Registry &registry, float dt)
{
    registry.view<Position, Physics>().each([dt](auto entity, Position &position, Physics &physics)
    {
        auto x = physics.x + physics.vx * dt;
        auto y = physics.y + physics.vy * dt;
        auto z = physics.z + physics.vz * dt;
        auto vz = physics.vz - 5.0f * dt;
        
        if (z < 0.0f)
        {
            z = -z;
            physics.vx *= 0.6f;
            physics.vy *= 0.6f;
            vz = -vz * 0.6f;
        }

        position.x = x;
        position.y = y - z;

        physics.x = x;
        physics.y = y;
        physics.z = z;
        physics.vz = vz;
    });
}
