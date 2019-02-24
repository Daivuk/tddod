#include "labels.h"
#include "ShapeRenderSystem.h"

#include "components/Color.h"
#include "components/Position.h"
#include "components/Resources.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"

#include "helpers/RenderingHelpers.h"

void updateShapeRenderSystem(Registry &registry)
{
    Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

    auto pVertices = resources.pPCVertices;

    registry.view<ShapeRenderer, Position, Size, Color>().each([&pVertices](auto entity, const ShapeRenderer& shapeRenderer, const Position& position, const Size& size, const Color& color)
    {
        pVertices += shapeRenderer.drawShape(pVertices, position, size.w, size.h, color);
    });

    auto count = pVertices - resources.pPCVertices;
    if (count) Rendering::drawLines(registry, resources.pPCVertices, count);
}
