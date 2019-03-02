#ifndef ENDWAVEDELAYSYSTEM_H_INCLUDED
#define ENDWAVEDELAYSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Adds a little delay at the end of a wave before starting next one
*/
void updateEndWaveDelaySystem(Registry &registry, float dt);

#endif
