#include "labels.h"

#include "components/Button.h"
#include "components/Clickable.h"
#include "components/Color.h"
#include "components/Hoverable.h"
#include "components/UIContext.h"
#include "systems/ButtonStateSystem.h"

void updateButtonStateSystem(Registry &registry)
{
    auto &uiContext = registry.get<UIContext>(registry.attachee<Label::UI_CONTEXT>());

    registry.view<Button, Color>().each([&uiContext, &registry](auto entity, const Button& button, Color& color)
    {
        if (registry.has<Hoverable, Clickable>(entity))
        {
            auto &clickable = registry.get<Clickable>(entity);
            auto &hoverable = registry.get<Hoverable>(entity);

            if (entity == uiContext.hover)
            {
                if (entity == uiContext.down)
                    color = clickable.downColor;
                else
                    color = hoverable.hoverColor;
            }
            else
            {
                if (entity == uiContext.down)
                    color = hoverable.hoverColor;
                else
                    color = hoverable.normalColor;
            }
        }
        else
        {
            color = button.disabledColor;
        }
    });
}
