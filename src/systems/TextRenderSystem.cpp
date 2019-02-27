#include "tags.h"
#include "TextRenderSystem.h"

#include "components/Color.h"
#include "components/Position.h"
#include "components/Resources.h"
#include "components/TextRenderer.h"

#include "helpers/RenderingHelpers.h"

void updateTextRenderSystem(Registry &registry)
{
    Resources &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());

    glBindTexture(GL_TEXTURE_2D, resources.fontTexture);

    auto pVertices = resources.pPTCVertices;

    registry.view<TextRenderer, Position, Color>().each([&pVertices](auto entity, const TextRenderer &textRenderer, const Position &position, const Color &color)
    {
        pVertices += Rendering::drawText(pVertices, textRenderer.text, position, textRenderer.bgColor, color, textRenderer.align);
    });

    auto count = pVertices - resources.pPTCVertices;
    if (count) Rendering::drawQuads(registry, resources.pPTCVertices, count);
}
