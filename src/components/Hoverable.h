/*
    The entity is hoverable with the mouse, and changes his color if hovered.
*/

#ifndef HOVERABLE_H_INCLUDED
#define HOVERABLE_H_INCLUDED

#include "Color.h"

struct Hoverable
{
    Color normalColor;
    Color hoverColor;
};

#endif
