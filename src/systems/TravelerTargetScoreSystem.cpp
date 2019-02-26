#include "components/Position.h"
#include "components/TargetScore.h"
#include "components/Traveler.h"
#include "systems/TravelerTargetScoreSystem.h"

void updateTravelerTargetScoreSystem(Registry &registry)
{
    registry.view<Traveler, Position, TargetScore>().each([](auto entity, const Traveler &traveler, const Position &position, TargetScore &targetScore)
    {
        auto tilesPercent = (float)traveler.nextWaypointIndex / (float)traveler.waypointCount;
        const auto &lastWaypointPos = traveler.pWaypoints[traveler.nextWaypointIndex - 1];
        const auto &nextWaypointPos = traveler.pWaypoints[traveler.nextWaypointIndex];
        auto dx = nextWaypointPos.x - lastWaypointPos.x;
        auto dy = nextWaypointPos.y - lastWaypointPos.y;
        auto tileSize = std::sqrtf(dx * dx + dy * dy);
        dx = lastWaypointPos.x - position.x;
        dy = lastWaypointPos.y - position.y;
        auto distToPrev = std::sqrtf(dx * dx + dy * dy);
        targetScore.score = tilesPercent + distToPrev / tileSize / (float)traveler.waypointCount;
    });
}
