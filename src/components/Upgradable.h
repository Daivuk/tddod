#ifndef UPGRADABLE_H_INCLUDED
#define UPGRADABLE_H_INCLUDED

#include "ecs.h"

typedef void(*UPGRADE_FN)(Entity entity);

struct Upgradable
{
    float cost;
    UPGRADE_FN upgrade;
};

#endif
