#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "ecs.h"

namespace Game
{
    void init(Registry &registry);
    void updateSim(Registry &registry, float dt);
    void update(Registry &registry, float dt);
    void render(Registry &registry);
};

#endif
