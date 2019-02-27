#include "constants.h"
#include "data.h"
#include "tags.h"

#include "components/EndWaveChecker.h"
#include "components/Position.h"
#include "components/Spawner.h"
#include "components/WavesController.h"
#include "components/Wave.h"
#include "helpers/WaveHelpers.h"

namespace Waves
{
    Entity createWavesController(Registry &registry)
    {
        auto entity = registry.create();
        registry.assign<Tag::WavesController>(entt::tag_t{}, entity);
        registry.assign<WavesController>(entity, 0, WAVE_COUNT, (Wave*)WAVES);
        return entity;
    }

    void startWave(Registry &registry, int index)
    {
        auto wavesControllerEntity  = registry.attachee<Tag::WavesController>();
        auto &wavesController       = registry.get<WavesController>(wavesControllerEntity);

        registry.assign<EndWaveChecker>(wavesControllerEntity);

        // Spawner
        {
            auto entity = registry.create();
            registry.assign<Wave>(entity, Wave{ wavesController.pWaves[wavesController.currentWaveIndex] });
            registry.assign<Position>(entity, WAVE_START_POSITION);
            registry.assign<Spawner>(entity, WAVE_START_TIME);
        }
    }
};
