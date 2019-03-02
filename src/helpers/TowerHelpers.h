/*
    Tower factories
*/

#ifndef TOWERHELPERS_H_INCLUDED
#define TOWERHELPERS_H_INCLUDED

#include "ecs.h"

#include "components/Placer.h"

struct Position;

namespace Tower
{
    /*
        Create a tower placer cursor with a callback (Could be use for other
        things than towers)
    */
    void createPlacer(Registry &registry, PLACE_FN place);

    /*
        Tower factories that also perform the purchase from Player bank.
        If can't purchase, it won't create the tower.
    */
    void buyGunTurret(Registry &registry, const Position &position);
    void buySlowTurret(Registry &registry, const Position &position);
    void buyRocketTurret(Registry &registry, const Position &position);
    void buyCannonTurret(Registry &registry, const Position &position);
};

#endif
