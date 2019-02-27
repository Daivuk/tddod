/*
    Store information about the current wave.
    Wave is defined as a series of keyframes.

    Note: This is only used by the Spawner component, so it could be stored
    in it directly, but it's also used by data.h as generic struct to define
    the waves.
*/

#ifndef WAVE_H_INCLUDED
#define WAVE_H_INCLUDED

#include "helpers/CritterHelpers.h"

struct Wave
{
    int                         count;              // Keyframe count
    const float                *pTimes;             // Array to keyframe time
    const CREATE_CRITTER_FN    *pCritterFactories;  // Func to call on keyframe
};

#endif
