#include "constants.h"
#include "data.h"
#include "tags.h"

#include "components/Inputs.h"
#include "components/Position.h"
#include "systems/InputSystem.h"

static InputState updateInputState(bool raw, InputState prevState)
{
    if (raw)
    {
        if (prevState != InputState::Down) return InputState::JustDown;
        else return prevState;
    }
    else
    {
        if (prevState != InputState::Up) return InputState::JustUp;
        else return prevState;
    }
}

void updateInputSystem(Registry &registry)
{
    auto &inputs = registry.get<Inputs>(registry.attachee<Tag::Inputs>());

    // Generate mouse world position from mouse position
    inputs.worldMouse.x = (inputs.mouse.x - ((float)WIDTH - MAP_WIDTH * ZOOM) / 2.0f) / ZOOM + 0.5f;
    inputs.worldMouse.y = (inputs.mouse.y - ((float)HEIGHT - MAP_HEIGHT * ZOOM) / 2.0f) / ZOOM + 0.4f;

    inputs.leftButtonState = updateInputState(inputs.leftButton, inputs.leftButtonState);
    inputs.rightButtonState = updateInputState(inputs.rightButton, inputs.rightButtonState);
    inputs.upgradeKeyState = updateInputState(inputs.upgradeKey, inputs.upgradeKeyState);
}
