/*
    Defines the entity as a target.
*/

#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include <cinttypes>

// One or a combinaison
namespace TargetMask
{
    static const uint8_t GROUND = 0x01; // Ground entity
    static const uint8_t AIR    = 0x02; // Air entity
};

struct Target
{
    uint8_t mask;
};

#endif
