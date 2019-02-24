#include <SDL_keycode.h>
#include <SDL_mouse.h>

#include "labels.h"

#include "components/Inputs.h"
#include "helpers/InputHelpers.h"

namespace Input
{
    void onMouseButtonDown(Registry &registry, int button)
    {
        auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());
        if (button == SDL_BUTTON_LEFT) inputs.leftButton = true;
        else if (button == SDL_BUTTON_RIGHT) inputs.rightButton = true;
    }

    void onMouseButtonUp(Registry &registry, int button)
    {
        auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());
        if (button == SDL_BUTTON_LEFT) inputs.leftButton = false;
        else if (button == SDL_BUTTON_RIGHT) inputs.rightButton = false;
    }

    void onMouseMotion(Registry &registry, int x, int y)
    {
        auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());
        inputs.mouse.x = (float)x;
        inputs.mouse.y = (float)y;
    }

    void onKeyDown(Registry &registry, int key)
    {
        auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());

        switch (key)
        {
        case SDLK_u: inputs.upgradeKey = true;
        }
    }

    void onKeyUp(Registry &registry, int key)
    {
        auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());

        switch (key)
        {
        case SDLK_u: inputs.upgradeKey = false;
        }
    }
};
