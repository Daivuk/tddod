#ifndef POSITIONANIMCALLBACK_H_INCLUDED
#define POSITIONANIMCALLBACK_H_INCLUDED

#include "ecs.h"

#include "components/Position.h"

typedef void(*POSITION_ANIM_CALLBACK_FN)(Registry &, Entity);

struct PositionAnimCallback
{
    POSITION_ANIM_CALLBACK_FN callback;
};

#endif