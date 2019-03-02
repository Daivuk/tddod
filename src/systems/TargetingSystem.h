#ifndef TARGETINGSYSTEM_H_INCLUDED
#define TARGETINGSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Update targeters, find targets, shoot then change to cooldown state
*/
void updateTargetingSystem(Registry &registry, float dt);

#endif
