#include "StaticLinesRenderSystem.h"

#include "components/StaticLinesRenderer.h"

void updateStaticLinesRenderSystem(Registry &registry)
{
    registry.view<StaticLinesRenderer>().each([](auto entity, const auto &staticLinesRenderer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, staticLinesRenderer.vertexBuffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, (float*)(uintptr_t)(0));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (float*)(uintptr_t)(8));
        glDrawArrays(GL_LINES, 0, staticLinesRenderer.vertCount);
    });
}
