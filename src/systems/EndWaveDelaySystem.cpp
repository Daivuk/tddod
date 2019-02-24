#include "constants.h"
#include "data.h"
#include "EndWaveDelaySystem.h"
#include "labels.h"

#include "components/EndWaveDelay.h"
#include "components/Position.h"
#include "components/Spawner.h"
#include "components/Wave.h"
#include "components/WavesController.h"
#include "helpers/WaveHelpers.h"

void updateEndWaveDelaySystem(Registry &registry, float dt)
{
    registry.view<EndWaveDelay>().each([&registry, dt](auto entity, EndWaveDelay &endWaveDelay)
    {
        auto delay = endWaveDelay.delay;
        delay -= dt;
        if (delay <= 0.0f)
        {
            registry.remove<EndWaveDelay>(entity);

            auto wavesControllerEntity = registry.attachee<Label::WAVES_CONTROLLER>();
            auto &wavesController = registry.get<WavesController>(wavesControllerEntity);
            auto nextWaveIndex = wavesController.currentWaveIndex + 1;
            if (nextWaveIndex >= WAVE_COUNT)
            {
                registry.destroy(entity); // No more waves
            }
            else
            {
                wavesController.currentWaveIndex = nextWaveIndex;
                Waves::startWave(registry, wavesController.currentWaveIndex);
            }

            return;
        }
        endWaveDelay.delay = delay;
    });
}
