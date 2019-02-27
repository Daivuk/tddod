#include "HealthBarRenderSystem.h"
#include "tags.h"

#include "components/Health.h"
#include "components/LineRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"

#include "helpers/RenderingHelpers.h"

void updateHealthBarRenderSystem(Registry &registry)
{
    Resources &resources = registry.get<Resources>(registry.attachee<Tag::Resources>());

    auto pVertices = resources.pPCVertices;
    registry.view<Health, Position>().each([&pVertices](auto entity, const Health &health, const Position &position)
    {
        if (health.amount == health.max) return; // We don't show full health
        auto t = health.amount / health.max;
        pVertices += Rendering::drawRect(pVertices, position.x - 0.2f, position.y - 0.4f, 0.4f, 0.1f, Color{ 0, .25f, 0, 1 });
        pVertices += Rendering::drawRect(pVertices, position.x - 0.2f, position.y - 0.4f, 0.4f * t, 0.1f, Color{ 1, 0, 0, 1 });
    });

    auto count = pVertices - resources.pPCVertices;
    if (count) Rendering::drawQuads(registry, resources.pPCVertices, count);
}
