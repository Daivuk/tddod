#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include <cinttypes>

namespace TargetMask
{
    static const uint8_t GROUND = 0x01;
    static const uint8_t AIR = 0x02;
};

struct Target
{
    uint8_t mask;
};

#endif
