#include "HudSystem.h"
#include "labels.h"

#include "components/Bank.h"
#include "components/Hud.h"
#include "components/TextRenderer.h"
#include "components/WavesController.h"

void updateHudSystem(Registry &registry)
{
    auto &hud = registry.get<Hud>(registry.attachee<Label::HUD>());

    {
        auto &textRenderer = registry.get<TextRenderer>(hud.currentWaveLabel);
        auto wavesControllerEntity = registry.attachee<Label::WAVES_CONTROLLER>();
        if (wavesControllerEntity != NULL_ENTITY)
        {
            auto &wavesController = registry.get<WavesController>(wavesControllerEntity);
            textRenderer.text = "Wave: " + std::to_string(wavesController.currentWaveIndex + 1);
        }
        else
        {
            textRenderer.text = "Finished";
        }
    }
    {
        auto &playerBank = registry.get<Bank>(registry.attachee<Label::PLAYER>());
        auto &textRenderer = registry.get<TextRenderer>(hud.cashLabel);
        textRenderer.text = "Cash: $" + std::to_string((int)playerBank.amount);
    }
}
