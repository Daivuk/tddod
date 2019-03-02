#ifndef COOLDOWNSYSTEM_H_INCLUDED
#define COOLDOWNSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Update shooters (tower only for now) cooldown, then change back to
    shooting.
*/
void updateCooldownSystem(Registry &registry, float dt);

#endif
