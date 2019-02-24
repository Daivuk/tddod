#ifndef WAVEHELPERS_H_INCLUDED
#define WAVEHELPERS_H_INCLUDED

#include "ecs.h"

namespace Waves
{
    Entity createWavesController(Registry &registry);
    void startWave(Registry &registry, int index);
}

#endif
