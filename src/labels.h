#ifndef LABELS_H_INCLUDED
#define LABELS_H_INCLUDED

#include "ecs.h"

#pragma warning(push)
#pragma warning(disable: 4307) // EnTT throws a warning with the hashing here.

namespace Label
{
    using WAVES_CONTROLLER = entt::label<"WAVES"_hs>;
    using RESOURCES = entt::label<"RESOURCES"_hs>;
};

#pragma warning(pop)

#endif
