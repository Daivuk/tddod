/*
    Helper and factory functions related to shooting.
*/

#ifndef SHOOTINGHELPERS_H_INCLUDED
#define SHOOTINGHELPERS_H_INCLUDED

#include "ecs.h"

struct Position;
struct Color;

namespace Shooting
{
    void createSlowBolt(Registry &registry, const Position &from, const Position &to, const Color &color);
    void createCannonBall(Registry &registry, const Position &from, const Position &to, const Color &color, float damage);
    void createBullet(Registry &registry, const Position &from, const Position &to, const Color &color);
    void kill(Registry &registry, Entity target);
    void damage(Registry &registry, Entity target, float damage);
    void radiusDamage(Registry &registry, const Position &position, float radius, float damage);
    void slow(Registry &registry, const Position &position, float radius, float speedMult);
}

#endif
