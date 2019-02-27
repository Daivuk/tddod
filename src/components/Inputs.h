/*
    Singleton component, Tag::Input, for holding input states.
*/

#ifndef INPUTS_H_INCLUDED
#define INPUTS_H_INCLUDED

#include "components/Position.h"

enum class InputState
{
    Up,
    JustDown,
    Down,
    JustUp
};

struct Inputs
{
    // Mouse position
    Position        mouse;
    Position        worldMouse;

    // Button/Key states
    InputState      leftButtonState;
    InputState      rightButtonState;
    InputState      upgradeKeyState;

    // Raw states
    bool            leftButton;
    bool            rightButton;
    bool            upgradeKey;
};

#endif
