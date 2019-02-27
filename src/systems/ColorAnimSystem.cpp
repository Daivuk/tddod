#include "ColorAnimSystem.h"

#include "components/Color.h"
#include "components/ColorAnim.h"

void updateColorAnimSystem(Registry &registry, float dt)
{
    registry.view<ColorAnim, Color>().each([dt](auto entity, ColorAnim &colorAnim, Color &color)
    {
        auto anim = colorAnim.anim + dt;
        auto t = std::min(anim, colorAnim.duration) / colorAnim.duration;
        color.r = colorAnim.from.r + (colorAnim.to.r - colorAnim.from.r) * t;
        color.g = colorAnim.from.g + (colorAnim.to.g - colorAnim.from.g) * t;
        color.b = colorAnim.from.b + (colorAnim.to.b - colorAnim.from.b) * t;
        color.a = colorAnim.from.a + (colorAnim.to.a - colorAnim.from.a) * t;
        colorAnim.anim = anim;
    });
}
