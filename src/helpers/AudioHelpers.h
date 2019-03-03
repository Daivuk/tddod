#ifndef AUDIOHELPERS_H_INCLUDED
#define AUDIOHELPERS_H_INCLUDED

#include "ecs.h"
#include "components/Resources.h"

namespace Audio
{
    Sound loadSound(const char *filename);
    void playSound(Registry &registry, const Sound &sound);
}

#endif
