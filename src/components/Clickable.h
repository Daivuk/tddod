#ifndef CLICKABLE_H_INCLUDED
#define CLICKABLE_H_INCLUDED

#include "Color.h"
#include "ecs.h"

typedef void(*ON_CLICK_FN)(Registry &registry, Entity entity);

struct Clickable
{
    Color downColor;
    ON_CLICK_FN onClick;
};

#endif
