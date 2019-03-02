/*
    Helpers to manipulate money
*/

#ifndef BANKHELPERS_H_INCLUDED
#define BANKHELPERS_H_INCLUDED

#include "ecs.h"

namespace Money
{
    /*
        Withdraw @amount from @from's Bank component if enough money in it, then
        return true. Returns false otherwise and leave @from' Bank untouched.
    */
    bool withdraw(Registry &registry, Entity from, int amount);

    /*
        Transfer all the money from @from's Bank to @to's bank.
    */
    void transfer(Registry &registry, Entity from, Entity to);
};

#endif
