/*
    Placer listen to inputs to place something on the map and place() is then
    called.
*/

#ifndef PLACER_H_INCLUDED
#define PLACER_H_INCLUDED

struct Position;

typedef void(*PLACE_FN)(Registry &, const Position &);

struct Placer
{
    PLACE_FN place;
};

#endif
