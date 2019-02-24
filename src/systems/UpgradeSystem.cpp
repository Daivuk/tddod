#include "labels.h"

#include "components/Inputs.h"
#include "components/UIContext.h"
#include "components/Upgradable.h"
#include "systems/UpgradeSystem.h"
#include "helpers/BankHelpers.h"

void updateUpgradeSystem(Registry &registry)
{
    auto &inputs = registry.get<Inputs>(registry.attachee<Label::INPUTS>());
    auto &uiContext = registry.get<UIContext>(registry.attachee<Label::UI_CONTEXT>());

    if (inputs.upgradeKeyState == InputState::JustDown &&
        uiContext.hover != NULL_ENTITY)
    {
        if (registry.has<Upgradable>(uiContext.hover))
        {
            const auto& upgradable = registry.get<Upgradable>(uiContext.hover);

            if (Money::withdraw(registry, registry.attachee<Label::PLAYER>(), (int)upgradable.cost))
            {
                upgradable.upgrade(registry, uiContext.hover);
            }
        }
    }
}
