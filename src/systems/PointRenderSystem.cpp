#include "tags.h"
#include "LineRenderSystem.h"

#include "components/Color.h"
#include "components/PointRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"

#include "helpers/RenderingHelpers.h"

void updatePointRenderSystem(Registry &registry)
{
    Resources &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());

    auto pVertices = resources.pPCVertices;

    registry.view<PointRenderer, Position, Color>().each([&pVertices](auto entity, const PointRenderer &pointRenderer, const Position &position, const Color &color)
    {
        pVertices->position = position;
        pVertices->color = color;
        ++pVertices;
    });

    auto count = pVertices - resources.pPCVertices;
    if (count) Rendering::drawPoints(registry, resources.pPCVertices, count);
}
