#ifndef TRAVELER_H_INCLUDED
#define TRAVELER_H_INCLUDED

struct Position;

struct Traveler
{
    float speed;
    int nextWaypointIndex;
    int waypointCount;
    Position *pWaypoints;
};

#endif
