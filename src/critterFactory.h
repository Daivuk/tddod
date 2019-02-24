#ifndef CRITTERFACTORY_H_INCLUDED
#define CRITTERFACTORY_H_INCLUDED

struct Position;

typedef void(*CRITTER_FACTORY)(const Position&);

void createPeasant(const Position& position);
void createWarrior(const Position& position);
void createThief(const Position& position);
void createFlyer(const Position& position);
void createBehemoth(const Position& position);

#endif
