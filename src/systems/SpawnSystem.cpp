#include "SpawnSystem.h"

#include "components/EndWaveDelay.h"
#include "components/Position.h"
#include "components/Spawner.h"
#include "components/Wave.h"

void updateSpawnSystem(Registry &registry, float dt)
{
    registry.view<Wave, Spawner, Position>().each([&registry, dt](auto entity, const Wave &wave, Spawner &spawner, const Position &position)
    {
        // If no more waves, return
        auto waveTime = spawner.waveTime;
        auto lastTime = waveTime;
        waveTime += dt;

        // Check all time keys to see if we should spawn critters
        for (int i = 0, len = wave.count; i < len; ++i)
        {
            auto t = wave.pTimes[i];
            if (lastTime < t && waveTime >= t)
            {
                auto spawnFn = wave.pCritterFactories[i];
                if (spawnFn)
                {
                    // Spawn
                    spawnFn(registry, position);
                }
                else
                {
                    registry.destroy(entity);
                    return;
                }
            }
        }

        spawner.waveTime = waveTime;
    });
}
