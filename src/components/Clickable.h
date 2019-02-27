/*
    Clickable entity with the mouse.
*/

#ifndef CLICKABLE_H_INCLUDED
#define CLICKABLE_H_INCLUDED

#include "Color.h"
#include "ecs.h"

typedef void(*ON_CLICK_FN)(Registry &, Entity);

struct Clickable
{
    Color       downColor;
    ON_CLICK_FN onClick;
};

#endif
