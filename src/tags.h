/*
    Tag names to singleton entities.
*/

#ifndef TAGS_H_INCLUDED
#define TAGS_H_INCLUDED

#include "ecs.h"

#pragma warning(push)
#pragma warning(disable: 4307) // EnTT throws a warning with the hashing here in VS2017

#define LABEL(__name__) using __name__ = entt::label<#__name__ ## _hs>

namespace Tag
{
    LABEL(WavesController);
    LABEL(Resources);
    LABEL(Hud);
    LABEL(Player);
    LABEL(UIContext);
    LABEL(Inputs);
    LABEL(Tooltip);
};

#pragma warning(pop)

#endif
