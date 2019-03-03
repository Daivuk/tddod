/*
    Main loop calls into this namespace.
    The Game sould probably be an object that hold his own Registry, but
    for simplicity, we only use 1 global Registry declared in main.
*/

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "ecs.h"

namespace Game
{
    void init(Registry &registry);
    void updateSim(Registry &registry, float dt);
    void update(Registry &registry, float dt);
    void updateAudio(Registry &registry, uint8_t *buffer, int len);
    void render(Registry &registry);
};

#endif
