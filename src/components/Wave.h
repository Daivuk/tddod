#ifndef WAVE_H_INCLUDED
#define WAVE_H_INCLUDED

#include "helpers/CritterHelpers.h"

struct Wave
{
    int count;
    const float *pTimes;
    const CREATE_CRITTER_FN *pCritterFactories;
};

#endif
