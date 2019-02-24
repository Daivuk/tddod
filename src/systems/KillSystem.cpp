#include "KillSystem.h"

#include "components/Dead.h"

void updateKillSystem(Registry &registry)
{
    registry.destroy<Dead>();
}
