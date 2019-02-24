#ifndef BANKHELPERS_H_INCLUDED
#define BANKHELPERS_H_INCLUDED

#include "ecs.h"

namespace Money
{
    bool withdraw(Registry &registry, Entity from, int amount);
    void transfer(Registry &registry, Entity from, Entity to);
};

#endif
