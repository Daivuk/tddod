#include "SizePulseAnimSystem.h"

#include "components/Size.h"
#include "components/SizePulseAnim.h"

void updateSizePulseAnimSystem(Registry &registry, float dt)
{
    registry.view<SizePulseAnim, Size>().each([dt](auto entity, SizePulseAnim &sizePulseAnim, Size &size)
    {
        auto anim = sizePulseAnim.anim + sizePulseAnim.speed * dt;
        auto t = std::sinf(anim);
        auto r = (sizePulseAnim.to - sizePulseAnim.from) / 2.0f;
        auto sizeA = sizePulseAnim.from + r + t * r;
        sizePulseAnim.anim = anim;
        size.w = sizeA;
        size.h = sizeA;
    });
}
