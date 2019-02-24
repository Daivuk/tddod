#include <cmath>

#include "components.h"
#include "shapes.h"

#include "helpers/RenderingHelpers.h"

static const float PI2 = 3.1415926535897932384626433832795f * 2.0f;

int drawBox(VertexPC *pVertices, const Position& position, float w, float h, const Color& color)
{
    pVertices[0].position.x = position.x - w;
    pVertices[0].position.y = position.y - h;
    pVertices[0].color = color;
    pVertices[1].position.x = position.x + w;
    pVertices[1].position.y = position.y - h;
    pVertices[1].color = color;

    pVertices[2].position.x = position.x - w;
    pVertices[2].position.y = position.y + h;
    pVertices[2].color = color;
    pVertices[3].position.x = position.x + w;
    pVertices[3].position.y = position.y + h;
    pVertices[3].color = color;

    pVertices[4].position.x = position.x - w;
    pVertices[4].position.y = position.y - h;
    pVertices[4].color = color;
    pVertices[5].position.x = position.x - w;
    pVertices[5].position.y = position.y + h;
    pVertices[5].color = color;

    pVertices[6].position.x = position.x + w;
    pVertices[6].position.y = position.y - h;
    pVertices[6].color = color;
    pVertices[7].position.x = position.x + w;
    pVertices[7].position.y = position.y + h;
    pVertices[7].color = color;

    return 8;
}

int drawCircle(VertexPC *pVertices, const Position& position, float w, float h, const Color& color)
{
    int vertCount = 0;
    const float step = PI2 / 12.0f;

    for (float i = 0.0f; i < PI2; i += step, ++vertCount)
    {
        pVertices[vertCount].position.x = position.x + std::cosf(i) * w; //TODO: Precompute cos/sin here because we always have fixed amount of verts
        pVertices[vertCount].position.y = position.y + std::sinf(i) * h;
        pVertices[vertCount].color = color;

        ++vertCount;
        pVertices[vertCount].position.x = position.x + std::cosf(i + step) * w;
        pVertices[vertCount].position.y = position.y + std::sinf(i + step) * h;
        pVertices[vertCount].color = color;
    }

    return vertCount;
}
