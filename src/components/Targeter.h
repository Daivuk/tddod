#ifndef TARGETER_H_INCLUDED
#define TARGETER_H_INCLUDED

#include "ecs.h"

typedef void(*SHOOT_FN)(Entity, Entity);

struct Targeter
{
    uint8_t targetMask;
    float range;
    float cooldown;
    SHOOT_FN shoot;
};

#endif
