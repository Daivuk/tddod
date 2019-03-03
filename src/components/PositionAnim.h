#ifndef POSITIONANIM_H_INCLUDED
#define POSITIONANIM_H_INCLUDED

#include "components/Position.h"

struct PositionAnim
{
    float anim;
    float duration;
    Position from;
    Position to;
};

#endif