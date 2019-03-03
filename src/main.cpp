/*
    Program entry and main loop.
    This is where we initialize SDL and poll events.
*/

#if defined(WIN32)
#include <windows.h>
#endif

#include <GL/gl3w.h>
#include <SDL.h>

#include <chrono>
#include <stdio.h>

#include "constants.h"
#include "game.h"

#include "helpers/InputHelpers.h"

void audioCallback(void *userdata, Uint8 *stream, int len);

#if defined(WIN32)
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
#else
int main(int argc, char** argv)
#endif
{
    // Init SDL
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    auto sdlWindow = SDL_CreateWindow(
        "Data Oriented Tower Defense",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Init OpenGL
    auto glContext = SDL_GL_CreateContext(sdlWindow);
    SDL_GL_SetSwapInterval(1);
    gl3wInit();

    // Create our registry and initialize the game
    Registry registry;
    Game::init(registry);

    // Init audio
    SDL_AudioSpec audioSpec;
    memset(&audioSpec, 0, sizeof(SDL_AudioSpec));
    audioSpec.freq = 44100;
    audioSpec.format = AUDIO_S16LSB;
    audioSpec.callback = audioCallback;
    audioSpec.channels = 2;
    audioSpec.samples = 4096;
    audioSpec.userdata = &registry;
    if (SDL_OpenAudio(&audioSpec, NULL) < 0)
    {
        assert(false);
    }
    SDL_PauseAudio(0);

    // Main loop
    int updateSpeed = 1;
    bool done = false;
    auto lastTime = std::chrono::high_resolution_clock::now();
    while (!done)
    {
        // Calculate delta time
        auto now = std::chrono::high_resolution_clock::now();
        auto diffTime = now - lastTime;
        lastTime = now;
        auto dt = (float)((double)std::chrono::duration_cast<std::chrono::microseconds>(diffTime).count() / 1000000.0);

        // Poll events
        SDL_LockAudio();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_TAB) updateSpeed = 10;
                else if (event.key.keysym.sym == SDLK_SPACE)
                {
                    if (updateSpeed) updateSpeed = 0;
                    else updateSpeed = 1;
                }
                Input::onKeyDown(registry, event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_TAB) updateSpeed = 1;
                Input::onKeyUp(registry, event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                Input::onMouseButtonDown(registry, event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                Input::onMouseButtonUp(registry, event.button.button);
                break;
            case SDL_MOUSEMOTION:
                Input::onMouseMotion(registry, event.motion.x, event.motion.y);
                break;
            }
        }

        // Update game simulation
        for (int i = 0; i < updateSpeed; ++i)
        {
            Game::updateSim(registry, dt);
        }

        // Update UI stuff, independent from simulation
        Game::update(registry, dt);

        // Draw game
        Game::render(registry);

        SDL_UnlockAudio();

        // Swap buffers
        SDL_GL_SwapWindow(sdlWindow);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void audioCallback(void *userdata, Uint8 *stream, int len)
{
    memset(stream, 0, len);

    auto &registry = *(Registry*)userdata;
    Game::updateAudio(registry, stream, len);
}
