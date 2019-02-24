#ifndef SHOOTINGHELPERS_H_INCLUDED
#define SHOOTINGHELPERS_H_INCLUDED

#include "ecs.h"

struct Position;
struct Color;

namespace Shooting
{
    void createBullet(Registry &registry, const Position& from, const Position& to, const Color& color);
    void kill(Registry &registry, Entity target);
    void damage(Registry &registry, Entity target, float damage);
}

#endif
