#include <SDL.h>

#include "components/SoundInstance.h"
#include "systems/AudioMixerSystem.h"

void updateAudioMixerSystem(Registry &registry, uint8_t *buffer, int len)
{
    registry.view<SoundInstance>().each([&registry, buffer, len](auto entity, SoundInstance &soundInstance)
    {
        auto clampedLen = (len > soundInstance.len ? soundInstance.len : len);
        SDL_MixAudio(buffer, soundInstance.buffer, clampedLen, SDL_MIX_MAXVOLUME / 2);
        
        soundInstance.buffer += clampedLen;
        soundInstance.len -= clampedLen;

        if (!soundInstance.len) registry.destroy(entity);
    });
}
