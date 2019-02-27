/*
    Travels along a path
*/

#ifndef TRAVELER_H_INCLUDED
#define TRAVELER_H_INCLUDED

struct Position;

struct Traveler
{
    int         nextWaypointIndex;  // Next path waypoint index
    int         waypointCount;      // Way point counts
    Position   *pWaypoints;         // Array of waypoints for the whole path
};

#endif
