#include "tags.h"
#include "LineRenderSystem.h"

#include "components/Color.h"
#include "components/LineRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"

#include "helpers/RenderingHelpers.h"

void updateLineRenderSystem(Registry &registry)
{
    Resources &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());

    auto pVertices = resources.pPCVertices;

    registry.view<Position, LineRenderer, Color>().each([&pVertices](auto entity, const Position &position, const LineRenderer &lineRenderer, const Color &color)
    {
        pVertices += Rendering::drawLine(pVertices, position, lineRenderer.target, color);
    });

    auto count = pVertices - resources.pPCVertices;
    if (count) Rendering::drawLines(registry, resources.pPCVertices, count);
}
