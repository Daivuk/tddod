/*
    Component that draws static lines.
    This is used by the map entity.
*/

#ifndef STATICLINESRENDERER_H_INCLUDED
#define STATICLINESRENDERER_H_INCLUDED

#include <GL/gl3w.h>

struct StaticLinesRenderer
{
    GLuint  vertexBuffer;
    GLsizei vertCount;
};

#endif
