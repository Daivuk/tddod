#ifndef UIHELPERS_H_INCLUDED
#define UIHELPERS_H_INCLUDED

#include "ecs.h"

#include "components/Clickable.h"

struct Color;
struct Position;

namespace UI
{
    Entity createLabel(Registry &registry, const char *text, const Position &position, const Color &color, float align = 0.0f);
    Entity createTowerButton(Registry &registry, const char *caption, const Position &position, int price, ON_CLICK_FN onClick);
};

#endif
