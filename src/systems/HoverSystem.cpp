#include "labels.h"

#include "components/Hoverable.h"
#include "components/Inputs.h"
#include "components/Size.h"
#include "components/UIContext.h"
#include "systems/HoverSystem.h"

void updateHoverSystem(Registry &registry)
{
    auto &inputs    = registry.get<Inputs>(registry.attachee<Label::INPUTS>());
    auto &uiContext = registry.get<UIContext>(registry.attachee<Label::UI_CONTEXT>());

    uiContext.hover = NULL_ENTITY;

    // Rect check
    registry.view<Hoverable, Position, Size>().each([&inputs, &uiContext](auto entity, const Hoverable &hoverable, const Position &position, const Size &size)
    {
        if (inputs.worldMouse.x >= position.x - size.w &&
            inputs.worldMouse.x <= position.x + size.w &&
            inputs.worldMouse.y >= position.y - size.h &&
            inputs.worldMouse.y <= position.y + size.h)
        {
            uiContext.hover = entity;
        }
    });
}
