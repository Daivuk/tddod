#include "TravelSystem.h"

#include "components/Position.h"
#include "components/Speed.h"
#include "components/Traveler.h"

void updateTravelSystem(Registry &registry, float dt)
{
    registry.view<Traveler, Position, Speed>().each([&registry, dt](auto entity, Traveler &traveler, Position &position, const Speed &speed)
    {
        auto i = traveler.nextWaypointIndex;
        auto curPosition = position;
        auto nextPosition = traveler.pWaypoints[i];
        auto dx = nextPosition.x - curPosition.x;
        auto dy = nextPosition.y - curPosition.y;
        auto dis = std::sqrtf(dx * dx + dy * dy);
        auto newDis = dis - speed.speed * dt;

        while (newDis <= 0.0f)
        {
            ++i;
            if (i >= traveler.waypointCount)
            {
                // Remove this entity. We have reached the end (Remove a health to the player also)
                registry.destroy(entity);
                return;
            }

            nextPosition = traveler.pWaypoints[i];
            dx = nextPosition.x - curPosition.x;
            dy = nextPosition.y - curPosition.y;
            dis = std::sqrtf(dx * dx + dy * dy);
            newDis += dis;
        }

        curPosition.x = nextPosition.x - dx / dis * newDis;
        curPosition.y = nextPosition.y - dy / dis * newDis;

        traveler.nextWaypointIndex = i;
        position = curPosition;
    });
}
