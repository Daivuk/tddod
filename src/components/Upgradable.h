/*
    Upgradable with the U key.
*/

#ifndef UPGRADABLE_H_INCLUDED
#define UPGRADABLE_H_INCLUDED

#include "ecs.h"

typedef void(*UPGRADE_FN)(Registry &, Entity);

struct Upgradable
{
    float       cost;
    UPGRADE_FN  upgrade;
};

#endif
