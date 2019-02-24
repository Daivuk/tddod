#ifndef LABELS_H_INCLUDED
#define LABELS_H_INCLUDED

#include "ecs.h"

#pragma warning(push)
#pragma warning(disable: 4307) // EnTT throws a warning with the hashing here in VS2017

#define LABEL(__name__) using __name__ = entt::label<#__name__ ## _hs>

namespace Label
{
    LABEL(WAVES_CONTROLLER);
    LABEL(RESOURCES);
    LABEL(HUD);
    LABEL(PLAYER);
    LABEL(UI_CONTEXT);
    LABEL(INPUTS);
    LABEL(TOOLTIP);
};

#pragma warning(pop)

#endif
