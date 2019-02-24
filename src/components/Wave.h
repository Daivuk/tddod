#ifndef WAVE_H_INCLUDED
#define WAVE_H_INCLUDED

#include "critterFactory.h"

struct Wave
{
    int count;
    const float *pTimes;
    const CRITTER_FACTORY *pCritterFactories;
};

#endif
