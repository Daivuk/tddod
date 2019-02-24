#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <GL/gl3w.h>

struct VertexPC;
struct VertexPTC;

struct Resources
{
    GLuint      programPC;      /* Position, Color */
    GLuint      programPTC;     /* Position, TexCoord, Color */
    GLuint      fontTexture;
    GLuint      vertexBuffer;   /* Dynamic version buffer used by batches */
    VertexPC   *pPCVertices;    /* Used by dynamic rendering of Position/Color */
    VertexPTC  *pPTCVertices;   /* Used by dynamic rendering of Position/TexCoord/Color */
};

#endif
