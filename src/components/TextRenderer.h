/*
    Renders Text at Position with Color
*/

#ifndef TEXTRENDERER_H_INCLUDED
#define TEXTRENDERER_H_INCLUDED

#include <string>

#include "components/Color.h"

struct TextRenderer
{
    std::string text;       // Text to display
    Color       bgColor;    // Background color
    float       align;      // Alignement. 0 = left, 1 = right, 0.5 = center
};

#endif
