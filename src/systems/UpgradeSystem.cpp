#include "tags.h"

#include "components/Inputs.h"
#include "components/UIContext.h"
#include "components/Upgradable.h"
#include "systems/UpgradeSystem.h"
#include "helpers/BankHelpers.h"

void updateUpgradeSystem(Registry &registry)
{
    auto &inputs = registry.get<Inputs>(registry.attachee<Tag::Inputs>());
    auto &uiContext = registry.get<UIContext>(registry.attachee<Tag::UIContext>());

    if (inputs.upgradeKeyState == InputState::JustDown &&
        uiContext.hover != NULL_ENTITY)
    {
        if (registry.has<Upgradable>(uiContext.hover))
        {
            const auto &upgradable = registry.get<Upgradable>(uiContext.hover);

            if (Money::withdraw(registry, registry.attachee<Tag::Player>(), (int)upgradable.cost))
            {
                upgradable.upgrade(registry, uiContext.hover);
                inputs.upgradeKeyState = InputState::Down; // Consume input
            }
        }
    }
}
