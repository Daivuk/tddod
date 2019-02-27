#ifndef TRAVELER_H_INCLUDED
#define TRAVELER_H_INCLUDED

struct Position;

struct Traveler
{
    int nextWaypointIndex;
    int waypointCount;
    Position *pWaypoints;
};

#endif
