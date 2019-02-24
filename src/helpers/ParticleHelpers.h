#ifndef PARTICLEHELPERS_H_INCLUDED
#define PARTICLEHELPERS_H_INCLUDED

#include "ecs.h"

#include "components/Color.h"
#include "components/Position.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"

namespace PFX
{
    void spawnParticlesFromShape(Registry &registry, const Position& position, const ShapeRenderer& shapeRenderer, const Size& size, const Color& color);
};

#endif
