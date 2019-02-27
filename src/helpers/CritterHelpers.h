#ifndef CRITTERHELPERS_H_INCLUDED
#define CRITTERHELPERS_H_INCLUDED

#include "ecs.h"

struct Position;

typedef void(*CREATE_CRITTER_FN)(Registry &, const Position&);

namespace Critters
{
    void createPeasant(Registry &registry, const Position &position);
    void createWarrior(Registry &registry, const Position &position);
    void createThief(Registry &registry, const Position &position);
    void createFlyer(Registry &registry, const Position &position);
    void createBehemoth(Registry &registry, const Position &position);
};

#endif
