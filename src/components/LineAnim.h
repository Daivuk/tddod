/*
    Animates LineRenderer components.
*/

#ifndef LINEANIM_H_INCLUDED
#define LINEANIM_H_INCLUDED

#include "Position.h"

struct LineAnim
{
    float       anim;
    float       duration;
    Position    fromFrom;
    Position    fromTo;
    Position    toFrom;
    Position    toTo;
};

#endif
