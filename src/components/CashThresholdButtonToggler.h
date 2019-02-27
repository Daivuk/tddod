/*
    Will toggle the owner button depending on the cash threshold held
    by the player. Used by tower purchase buttons to grey out when the player
    doesn't have enough money.
*/

#ifndef CASHTHRESHOLDBUTTONTOGGLER_H_INCLUDED
#define CASHTHRESHOLDBUTTONTOGGLER_H_INCLUDED

struct CashThresholdButtonToggler
{
    float threshold;
};

#endif
