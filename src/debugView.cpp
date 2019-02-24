#include <cinttypes>

#include "components/Bank.h"
#include "components/Button.h"
#include "components/CashThresholdButtonToggler.h"
#include "components/Clickable.h"
#include "components/Color.h"
#include "components/ColorAnim.h"
#include "components/Critter.h"
#include "components/Cooldown.h"
#include "components/Dead.h"
#include "components/DeathTimer.h"
#include "components/EndWaveChecker.h"
#include "components/EndWaveDelay.h"
#include "components/FindTarget.h"
#include "components/Health.h"
#include "components/Hoverable.h"
#include "components/Hud.h"
#include "components/Inputs.h"
#include "components/Investor.h"
#include "components/LineAnim.h"
#include "components/LineRenderer.h"
#include "components/Name.h"
#include "components/Physics.h"
#include "components/Placer.h"
#include "components/PointRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"
#include "components/SizePulseAnim.h"
#include "components/Spawner.h"
#include "components/StaticLinesRenderer.h"
#include "components/Target.h"
#include "components/Targeter.h"
#include "components/TexCoord.h"
#include "components/TextRenderer.h"
#include "components/Traveler.h"
#include "components/UIContext.h"
#include "components/Upgradable.h"
#include "components/Wave.h"
#include "components/WavesController.h"

#include "constants.h"
#include "data.h"
#include "debugView.h"
#include "ecs.h"
#include "labels.h"

#include "components/Resources.h"

#include "helpers/RenderingHelpers.h"

bool g_debugViewEnabled = false;

static double memMin = std::numeric_limits<double>::max();
static double memMax = 0.0f;

#define DRAW_COMP(__c__) \
{ \
    auto view = registry.view<__c__>(); \
    pVertices += Rendering::drawText( \
        pVertices, \
        std::string(#__c__) + "[" + std::to_string(registry.size<__c__>()) + "," + std::to_string(registry.capacity<__c__>()) + "]", \
        { (float)(i / 20) * 12.0f, (float)(i % 20) * (16.0f / 20.0f) }, \
        { 0, 0, 0, 0 }, \
        { 0.25f, 0.85f, 1.0f, 1.0f}, \
        0); \
    ++i; \
}

//view.each([&pVertices, color](auto entity, const __c__& c) \
//{ \
//auto addr = (double)(uintptr_t)&c; \
//memMin = std::min(memMin, addr); \
//memMax = std::max(memMax, addr); \
//auto p = (float)((addr - memMin) / (memMax - memMin) * 16.0); \
//pVertices += Rendering::drawRect(pVertices, 17, p, 2, 0.1f, color); \
//});

void drawDebugView(Registry &registry)
{
    glViewport(WIDTH, 0, WIDTH, HEIGHT);

    Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

    Rendering::prepareForPTC(registry);
    glBindTexture(GL_TEXTURE_2D, resources.fontTexture);

    auto pVertices = resources.pPTCVertices;

    int i = 0;

    DRAW_COMP(Bank);
    DRAW_COMP(Button);
    DRAW_COMP(CashThresholdButtonToggler);
    DRAW_COMP(Clickable);
    DRAW_COMP(Color);
    DRAW_COMP(ColorAnim);
    DRAW_COMP(Cooldown);
    DRAW_COMP(Critter);
    DRAW_COMP(Dead);
    DRAW_COMP(DeathTimer);
    DRAW_COMP(EndWaveChecker);
    DRAW_COMP(EndWaveDelay);
    DRAW_COMP(FindTarget);
    DRAW_COMP(Health);
    DRAW_COMP(Hoverable);
    DRAW_COMP(Hud);
    DRAW_COMP(Inputs);
    DRAW_COMP(Investor);
    DRAW_COMP(LineAnim);
    DRAW_COMP(LineRenderer);
    DRAW_COMP(Name);
    DRAW_COMP(Physics);
    DRAW_COMP(Placer);
    DRAW_COMP(PointRenderer);
    DRAW_COMP(Position);
    DRAW_COMP(Resources);
    DRAW_COMP(ShapeRenderer);
    DRAW_COMP(Size);
    DRAW_COMP(SizePulseAnim);
    DRAW_COMP(Spawner);
    DRAW_COMP(StaticLinesRenderer);
    DRAW_COMP(Target);
    DRAW_COMP(Targeter);
    DRAW_COMP(TexCoord);
    DRAW_COMP(TextRenderer);
    DRAW_COMP(Traveler);
    DRAW_COMP(UIContext);
    DRAW_COMP(Upgradable);
    DRAW_COMP(Wave);
    DRAW_COMP(WavesController);

    auto count = pVertices - resources.pPTCVertices;
    if (count) Rendering::drawQuads(registry, resources.pPTCVertices, count);
}
