#include "components/Button.h"
#include "components/CashThresholdButtonToggler.h"
#include "components/Color.h"
#include "components/Hoverable.h"
#include "components/Name.h"
#include "components/Position.h"
#include "components/ShapeREnderer.h"
#include "components/Size.h"
#include "components/TextRenderer.h"
#include "helpers/ShapeHelpers.h"
#include "helpers/UIHelpers.h"

namespace UI
{
    Entity createLabel(Registry &registry, const char *text, const Position &position, const Color &color, float align)
    {
        auto entity = registry.create();
        registry.assign<Position>(entity, position);
        registry.assign<Color>(entity, color);
        registry.assign<TextRenderer>(entity, text, Color{ 0,0,0,0 }, align);
        return entity;
    }

    Entity createTowerButton(Registry &registry, const char *caption, const Position &position, int price, ON_CLICK_FN onClick)
    {
        auto entity = registry.create();
        registry.assign<Position>(entity, position);
        registry.assign<Size>(entity, Size{ 2.5f, 0.4f });
        registry.assign<Color>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f });
        registry.assign<TextRenderer>(entity, caption, Color{ 0,0,0,0 }, 0.5f);
        registry.assign<Name>(entity, "$" + std::to_string(price));
        registry.assign<ShapeRenderer>(entity, Shape::drawBox);
        registry.assign<CashThresholdButtonToggler>(entity, (float)price);
        registry.assign<Button>(entity, Color{ 0.3f, 0.3f, 0.3f, 1 }, onClick);
        registry.assign<Hoverable>(entity, Color{ 0.6f, 0.7f, 0.5f, 1.0f }, Color{ 1, 1, 0.8f, 1 });
        return entity;
    }
};
