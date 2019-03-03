/*
    Singleton component, Tag::Resources, holding resource handles.
*/

#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <GL/gl3w.h>
#include <SDL.h>

struct VertexPC;
struct VertexPTC;

struct Sound
{
    Uint32 len;
    Uint8 *buffer;
    SDL_AudioSpec spec;
};

struct Resources
{
    GLuint      programPC;      /* Position, Color */
    GLuint      programPTC;     /* Position, TexCoord, Color */
    GLuint      fontTexture;
    GLuint      vertexBuffer;   /* Dynamic version buffer used by batches */
    VertexPC   *pPCVertices;    /* Used by dynamic rendering of Position/Color */
    VertexPTC  *pPTCVertices;   /* Used by dynamic rendering of Position/TexCoord/Color */
    Sound       gunSound;       /* When gun towers shoot */
    Sound       popSound;       /* Plays when enemy dies */
    Sound       slowSound;      /* Slow tower shoot */
    Sound       cannonSound;    /* Cannon shoot */
    Sound       explosionSound; /* Cannon ball impact */
};

#endif
