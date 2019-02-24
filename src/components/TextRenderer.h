#ifndef TEXTRENDERER_H_INCLUDED
#define TEXTRENDERER_H_INCLUDED

#include <string>

#include "components/Color.h"

struct TextRenderer
{
    std::string text;
    Color bgColor;
    float align;
};

#endif
