#ifndef INPUTHELPERS_H_INCLUDED
#define INPUTHELPERS_H_INCLUDED

#include "ecs.h"

namespace Input
{
    void onMouseButtonDown(Registry &registry, int button);
    void onMouseButtonUp(Registry &registry, int button);
    void onMouseMotion(Registry &registry, int x, int y);
    void onKeyDown(Registry &registry, int key);
    void onKeyUp(Registry &registry, int key);
};

#endif
