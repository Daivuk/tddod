#ifndef WAVESCONTROLLER_H_INCLUDED
#define WAVESCONTROLLER_H_INCLUDED

#include "ecs.h"

struct Wave;

struct WavesController
{
    int currentWaveIndex;
    int waveCount;
    Wave *pWaves;
    Entity waveLabel;
};

#endif
