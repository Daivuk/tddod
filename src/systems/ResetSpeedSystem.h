#ifndef RESETSPEEDSYSTEM_H_INCLUDED
#define RESETSPEEDSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Reset speed of things that have baseSpeed.
    This should be called before speed modifiers
*/
void updateResetSpeedSystem(Registry &registry);

#endif
