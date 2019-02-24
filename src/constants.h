#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

static const int WIDTH = 800;
static const int HEIGHT = 600;
static const float ZOOM = 32;

static const int MAX_VERTICES = 100000; // Grow this number if we need more
static const float TEXT_ADVANCE = 12.0f / ZOOM;

static const char *PC_VS =
    "uniform mat4 ProjMtx;\n"
    "in vec2 Position;\n"
    "in vec4 Color;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
    "}\n";

static const char *PC_PS =
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color;\n"
    "}\n";

static const char *PTC_VS =
    "uniform mat4 ProjMtx;\n"
    "in vec2 Position;\n"
    "in vec2 TexCoord;\n"
    "in vec4 Color;\n"
    "out vec2 Frag_TexCoord;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
    "    Frag_TexCoord = TexCoord;\n"
    "    Frag_Color = Color;\n"
    "}\n";

static const char *PTC_PS =
    "in vec2 Frag_TexCoord;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "uniform sampler2D Texture;\n"
    "void main()\n"
    "{\n"
    "    vec4 diffuse = texture2D(Texture, Frag_TexCoord);\n"
    "    Out_Color = diffuse * Frag_Color;\n"
    "}\n";

#endif
