#include "labels.h"

#include "components/Color.h"
#include "components/ColorAnim.h"
#include "components/DeathTimer.h"
#include "components/Physics.h"
#include "components/PointRenderer.h"
#include "components/Position.h"
#include "components/Resources.h"
#include "components/ShapeRenderer.h"
#include "components/Size.h"
#include "helpers/ParticleHelpers.h"
#include "helpers/RenderingHelpers.h"

namespace PFX
{
    void spawnParticlesFromShape(Registry &registry, const Position& position, const ShapeRenderer& shapeRenderer, const Size& size, const Color& color)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

        auto pVertices = resources.pPCVertices;
        pVertices += shapeRenderer.drawShape(pVertices, position, size.w, size.h, color);
        auto count = pVertices - resources.pPCVertices;
        for (int i = 0; i < count; i += 2)
        {
            const auto& vertex1 = resources.pPCVertices[i];
            const auto& vertex2 = resources.pPCVertices[i + 1];

            auto vdx = vertex2.position.x - vertex1.position.x;
            auto vdy = vertex2.position.y - vertex1.position.y;
            auto l = std::sqrtf(vdx * vdx + vdy * vdy);
            vdx = vdx / l * 0.025f;
            vdy = vdy / l * 0.025f;
            auto p = vertex1.position;

            for (float j = 0; j < l; j += 0.025f)
            {
                auto entity = registry.create();
                auto dx = p.x - position.x;
                auto dy = p.y - position.y;

                registry.assign<Position>(entity, p);
                registry.assign<PointRenderer>(entity);
                registry.assign<DeathTimer>(entity, 2.0f);
                registry.assign<Color>(entity, Color{ 1,1,1,1 });
                registry.assign<ColorAnim>(entity, 0.0f, 2.0f, vertex1.color, Color{ 0, 0, 0, 0 });
                registry.assign<Physics>(entity, p.x, p.y, 0.0f,
                    dx * 8.0f + (float)(rand() % 1000 - 500) / 3000.0f,
                    dy * 8.0f + (float)(rand() % 1000 - 500) / 3000.0f,
                    1.0f + (float)(rand() % 1000 - 500) / 3000.0f);

                p.x += vdx;
                p.y += vdy;
            }
        }
    }
};
