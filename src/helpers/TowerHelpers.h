#ifndef TOWERHELPERS_H_INCLUDED
#define TOWERHELPERS_H_INCLUDED

#include "ecs.h"

#include "components/Placer.h"

struct Position;

namespace Tower
{
    void createPlacer(Registry &registry, PLACE_FN place);

    void buyGunTurret(Registry &registry, const Position &position);
    void buySlowTurret(Registry &registry, const Position &position);
    void buyRocketTurret(Registry &registry, const Position &position);
    void buyCannonTurret(Registry &registry, const Position &position);
};

#endif
