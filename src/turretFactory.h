#ifndef TURRETFACTORY_H_INCLUDED
#define TURRETFACTORY_H_INCLUDED

struct Position;

typedef float(*TURRET_FACTORY)(const Position&); // Returns cost

void createGunTurret(const Position& position);
void createSlowTurret(const Position& position);
void createRocketTurret(const Position& position);
void createCannonTurret(const Position& position);

#endif
