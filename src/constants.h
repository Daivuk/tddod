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

// Shaders
static const char *PC_VS =
    "uniform mat4 ProjMtx;\n"
    "attribute vec2 Position;\n"
    "attribute vec4 Color;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
    "}\n";

static const char *PC_PS =
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = Frag_Color;\n"
    "}\n";

static const char *PTC_VS =
    "uniform mat4 ProjMtx;\n"
    "attribute vec2 Position;\n"
    "attribute vec2 TexCoord;\n"
    "attribute vec4 Color;\n"
    "varying vec2 Frag_TexCoord;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
    "    Frag_TexCoord = TexCoord;\n"
    "    Frag_Color = Color;\n"
    "}\n";

static const char *PTC_PS =
    "varying vec2 Frag_TexCoord;\n"
    "varying vec4 Frag_Color;\n"
    "uniform sampler2D Texture;\n"
    "void main()\n"
    "{\n"
    "    vec4 diffuse = texture2D(Texture, Frag_TexCoord);\n"
    "    gl_FragColor = diffuse * Frag_Color;\n"
    "}\n";

#include "data.h"

#endif
