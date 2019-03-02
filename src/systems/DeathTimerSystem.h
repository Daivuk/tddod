#ifndef DEATHTIMERSYSTEM_H_INCLUDED
#define DEATHTIMERSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Update death timers then kill finished entities
*/
void updateDeathTimerSystem(Registry &registry, float dt);

#endif
