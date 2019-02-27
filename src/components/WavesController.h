/*
    Singleton component, Tag::WavesController, storing waves' state.
*/

#ifndef WAVESCONTROLLER_H_INCLUDED
#define WAVESCONTROLLER_H_INCLUDED

#include "ecs.h"

struct Wave;

struct WavesController
{
    int     currentWaveIndex;   // Current wave index
    int     waveCount;          // How many waves for this level
    Wave   *pWaves;             // Array on waves
};

#endif
