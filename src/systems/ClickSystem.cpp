#include "tags.h"

#include "components/Clickable.h"
#include "components/Inputs.h"
#include "components/UIContext.h"
#include "systems/ClickSystem.h"

void updateClickSystem(Registry &registry)
{
    auto &inputs = registry.get<Inputs>(registry.attachee<Tag::Inputs>());
    auto &uiContext = registry.get<UIContext>(registry.attachee<Tag::UIContext>());

    if (inputs.leftButtonState == InputState::JustDown)
    {
        uiContext.down = uiContext.hover;
    }
    else if (inputs.leftButtonState == InputState::JustUp)
    {
        if (uiContext.down != NULL_ENTITY && 
            uiContext.down == uiContext.hover &&
            registry.has<Clickable>(uiContext.down))
        {
            auto &clickable = registry.get<Clickable>(uiContext.down);
            if (clickable.onClick) clickable.onClick(registry, uiContext.down);
            inputs.leftButtonState = InputState::Up; // Reset so it doesn't trigger again
        }
        uiContext.down = NULL_ENTITY;
    }
}
