#include <cinttypes>

#include "components.h"
#include "data.h"
#include "debugView.h"
#include "ecs.h"
#include "labels.h"

#include "components/Resources.h"

#include "helpers/RenderingHelpers.h"

bool g_debugViewEnabled = false;

extern GLuint g_programPC;
extern GLuint g_programPTC;
extern GLuint g_fontTexture;
extern GLuint g_shapeVB;
extern GLuint g_textVB;

extern VertexPC *g_pShapeVertices;
extern VertexPTC *g_pTextVertices;

static double memMin = std::numeric_limits<double>::max();
static double memMax = 0.0f;

static const Color COLORS[] = {
    { 1, 0, 0, 1 },
    { 0, 1, 0, 1 },
    { 0, 0, 1, 1 },
    { 1, 1, 0, 1 },
    { 1, 0, 1, 1 },
    { 0, 1, 1, 1 },
    { 1, 0.5f, 0.5f, 1 },
    { 0.5f, 1, 0.5f, 1 },
    { 0.5f, 0.5f, 1, 1 },
    { 1, 1, 0.5f, 1 },
    { 1, 0.5f, 1, 1 },
    { 0.5f, 1, 1, 1 },
    { 1, 1, 1, 1 },
};

#define DRAW_COMP(__c__, __y__) \
{ \
    auto view = g_registry.view<__c__>(); \
    Color color = COLORS[__y__ % (sizeof(COLORS) / sizeof(Color))]; \
    pVertices += Rendering::drawText( \
        pVertices, \
        std::string(#__c__) + "[" + std::to_string(g_registry.size<__c__>()) + "," + std::to_string(g_registry.capacity<__c__>()) + "]", \
        { 8, (float)__y__ * 0.5f }, \
        { 0, 0, 0, 0 }, \
        color, \
        0); \
    view.each([&pVertices, color](auto entity, const __c__& c) \
    { \
        auto addr = (double)(uintptr_t)&c; \
        memMin = std::min(memMin, addr); \
        memMax = std::max(memMax, addr); \
        auto p = (float)((addr - memMin) / (memMax - memMin) * 16.0); \
        pVertices += Rendering::drawRect(pVertices, 17, p, 2, 0.1f, color); \
    }); \
}

void drawDebugView()
{
    // Draw text
    {
        Resources &resources = g_registry.get<Resources>(g_registry.attachee<Label::RESOURCES>());

        Rendering::prepareForPTC(g_registry);
        glBindTexture(GL_TEXTURE_2D, resources.fontTexture);

        auto pVertices = resources.pPTCVertices;

        DRAW_COMP(Bank, 0);
        DRAW_COMP(Color, 1);
        DRAW_COMP(ColorAnim, 2);
        //DRAW_COMP(Delay, 1);
        //DRAW_COMP(DelayCallback, 1);
        DRAW_COMP(Health, 3);
        DRAW_COMP(Investor, 4);
        DRAW_COMP(LineAnim, 5);
        DRAW_COMP(LineRenderer, 6);
        DRAW_COMP(Name, 7);
        DRAW_COMP(Physics, 8);
        DRAW_COMP(Placer, 9);
        DRAW_COMP(PointRenderer, 10);
        DRAW_COMP(Position, 11);
        DRAW_COMP(ShapeRenderer, 12);
        DRAW_COMP(Size, 13);
        DRAW_COMP(SizePulseAnim, 14);
        DRAW_COMP(Targeter, 15);
        DRAW_COMP(TexCoord, 16);
        DRAW_COMP(Upgradable, 17);

        auto count = pVertices - resources.pPTCVertices;
        if (count) Rendering::drawQuads(g_registry, resources.pPTCVertices, count);
    }
}
