/*
    Entity that seeks a target.
*/

#ifndef TARGETER_H_INCLUDED
#define TARGETER_H_INCLUDED

#include "ecs.h"

typedef void(*SHOOT_FN)(Registry &, Entity, Entity);

struct Targeter
{
    uint8_t     targetMask; // See Target.h
    float       range;      // Seek range
    float       cooldown;   // Cooldown delay between shots
    SHOOT_FN    shoot;      // Function to call once target found
};

#endif
