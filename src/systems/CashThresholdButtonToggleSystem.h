#ifndef CASHTHRESHOLDBUTTONTOGGLESYSTEM_H_INCLUDED
#define CASHTHRESHOLDBUTTONTOGGLESYSTEM_H_INCLUDED

#include "ecs.h"

/*
    Toggles buttons from enable/disable state depending on the amount in 
    the player's bank. Used by tower buttons.
*/
void updateCashThresholdButtonToggleSystem(Registry &registry);

#endif
