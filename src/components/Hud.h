/*
    Singleton component, Tag::Hud.
*/

#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "ecs.h"

struct Hud
{
    Entity currentWaveLabel;
    Entity cashLabel;
};

#endif
