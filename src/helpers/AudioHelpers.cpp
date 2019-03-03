#include <cassert>

#include "components/SoundInstance.h"
#include "helpers/AudioHelpers.h"

namespace Audio
{
    Sound loadSound(const char *filename)
    {
        Sound ret;
        if (SDL_LoadWAV(filename, &ret.spec, &ret.buffer, &ret.len) == NULL)
        {
            assert(false);
        }
        return ret;
    }

    void playSound(Registry &registry, const Sound &sound)
    {
        auto entity = registry.create();
        registry.assign<SoundInstance>(entity, (int)sound.len, sound.buffer);
    }
};
