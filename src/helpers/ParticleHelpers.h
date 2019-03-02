/*
    Particle effects related helpers.
*/

#ifndef PARTICLEHELPERS_H_INCLUDED
#define PARTICLEHELPERS_H_INCLUDED

#include "ecs.h"

#include "components/Color.h"
#include "components/Position.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"

namespace PFX
{
    /*
        Create point particles along the edges of a shape with an outward 
        velocity.
    */
    void spawnParticlesFromShape(Registry &registry, const Position &position, const ShapeRenderer &shapeRenderer, const Size &size, const Color &color);
};

#endif
