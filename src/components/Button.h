#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "Clickable.h"

struct Button
{
    Color disabledColor;
    ON_CLICK_FN onClick;
};

#endif
