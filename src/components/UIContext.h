/*
    Singleton component, Tag::UIContext, storing UI states.
*/

#ifndef UICONTEXT_H_INCLUDED
#define UICONTEXT_H_INCLUDED

#include "ecs.h"

struct UIContext
{
    Entity hover;   // Current mouse hover entity
    Entity down;    // Entity when last mouse down
};

#endif
