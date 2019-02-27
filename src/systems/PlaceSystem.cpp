#include "data.h"
#include "tags.h"

#include "systems/PlaceSystem.h"

#include "components/Inputs.h"
#include "components/Placer.h"
#include "components/Position.h"

void updatePlaceSystem(Registry &registry)
{
    const auto &inputs = registry.get<Inputs>(registry.attachee<Tag::Inputs>());

    registry.view<Placer>().each([&registry, &inputs](auto entity, const Placer &placer)
    {
        int mx = (int)inputs.worldMouse.x;
        int my = (int)inputs.worldMouse.y;
        if (mx < 0 || mx >= MAP_WIDTH ||
            my < 0 || my >= MAP_HEIGHT)
        {
            // Invalid position
            registry.reset<Position>(entity);
        }
        else
        {
            Position position{ (float)mx + 0.5f, (float)my + 0.5f };

            if (inputs.leftButtonState == InputState::JustDown)
            {
                // Place
                placer.place(registry, position);
                registry.destroy(entity);
            }
            else
            {
                // Update his position component
                registry.accommodate<Position>(entity, position);
            }
        }

        // Cancel placing with right-click
        if (inputs.rightButtonState == InputState::JustDown)
        {
            registry.destroy(entity);
        }
    });
}
