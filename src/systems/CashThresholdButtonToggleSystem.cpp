#include "CashThresholdButtonToggleSystem.h"
#include "tags.h"

#include "components/Bank.h"
#include "components/Button.h"
#include "components/Clickable.h"
#include "components/Color.h"
#include "components/CashThresholdButtonToggler.h"
#include "components/Hoverable.h"

void updateCashThresholdButtonToggleSystem(Registry &registry)
{
    auto &playerBank = registry.get<Bank>(registry.attachee<Tag::Player>());

    registry.view<CashThresholdButtonToggler, Button, Color>().each([&registry, &playerBank](auto entity, const CashThresholdButtonToggler &cashThresholdButtonToggler, const Button &button, Color &color)
    {
        if (playerBank.amount >= cashThresholdButtonToggler.threshold)
        {
            registry.accommodate<Clickable>(entity, Color{ 0.5f, 0.5f, 0.4f, 1 }, button.onClick);
        }
        else
        {
            registry.reset<Clickable>(entity);
            color = button.disabledColor;
        }
    });
}
