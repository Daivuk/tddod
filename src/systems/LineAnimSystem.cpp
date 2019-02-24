#include "LineAnimSystem.h"

#include "components/LineAnim.h"
#include "components/LineRenderer.h"

void updateLineAnimSystem(Registry &registry, float dt)
{
    registry.view<LineAnim, Position, LineRenderer>().each([dt](auto entity, LineAnim &lineAnim, Position &position, LineRenderer &lineRenderer)
    {
        auto anim = lineAnim.anim + dt;
        auto t = std::min(anim, lineAnim.duration) / lineAnim.duration;
        position.x = lineAnim.fromFrom.x + (lineAnim.toFrom.x - lineAnim.fromFrom.x) * t;
        position.y = lineAnim.fromFrom.y + (lineAnim.toFrom.y - lineAnim.fromFrom.y) * t;
        lineRenderer.target.x = lineAnim.fromTo.x + (lineAnim.toTo.x - lineAnim.fromTo.x) * t;
        lineRenderer.target.y = lineAnim.fromTo.y + (lineAnim.toTo.y - lineAnim.fromTo.y) * t;
        lineAnim.anim = anim;
    });
}
