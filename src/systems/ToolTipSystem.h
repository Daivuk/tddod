#ifndef TOOLTIPSYSTEM_H_INCLUDED
#define TOOLTIPSYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Setup tooltip if the currently mouse hovered item has a name component.
*/
void updateToolTipSystem(Registry &registry);

#endif
