/*
    All constants and magic numbers in the game should be in this file.
    So if we need to tweak stuff, we only have one place to look.

    This could also make is easiest if we were to download settings from cloud,
    to have them all in one place already.
*/

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include "components/Color.h"

static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float ZOOM = 32;

static const int MAX_VERTICES = 100000; // Grow this number if we need more
static const float TEXT_ADVANCE = 12.0f / ZOOM;

static const float WAVE_START_TIME = -0.1f;

static const Color TOWER_LEVEL1_COLOR = { 0.15f, 1.0f, 0.15f, 1.0f };
static const Color TOWER_LEVEL2_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
static const Color TOWER_LEVEL3_COLOR = { 1.0f, 0.35f, 0.35f, 1.0f };

static const float TOWER_SIZE = 0.4f;

static const int GUN_TOWER_LVL1_PRICE = 100;
static const int GUN_TOWER_LVL2_PRICE = 200;
static const int GUN_TOWER_LVL3_PRICE = 400;

static const int SLOW_TOWER_LVL1_PRICE = 150;
static const int SLOW_TOWER_LVL2_PRICE = 300;
static const int SLOW_TOWER_LVL3_PRICE = 600;

static const int ROCKET_TOWER_LVL1_PRICE = 200;
static const int ROCKET_TOWER_LVL2_PRICE = 400;
static const int ROCKET_TOWER_LVL3_PRICE = 800;

static const int CANNON_TOWER_LVL1_PRICE = 250;
static const int CANNON_TOWER_LVL2_PRICE = 500;
static const int CANNON_TOWER_LVL3_PRICE = 1000;

#endif
