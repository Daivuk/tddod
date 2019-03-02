/*
    Wave control related helpers
*/

#ifndef WAVEHELPERS_H_INCLUDED
#define WAVEHELPERS_H_INCLUDED

#include "ecs.h"

namespace Waves
{
    /*
        Create the main wave controller. There can only be one.
    */
    Entity createWavesController(Registry &registry);

    /*
        Start a wave at index.
    */
    void startWave(Registry &registry, int index);
}

#endif
