#ifndef RENDERINGHELPERS_H_INCLUDED
#define RENDERINGHELPERS_H_INCLUDED

#include <GL/gl3w.h>

#include "ecs.h"

#include "components/Color.h"
#include "components/Position.h"
#include "components/TexCoord.h"

struct VertexPC
{
    Position position;
    Color color;
};

struct VertexPTC
{
    Position position;
    TexCoord texCoord;
    Color color;
};

namespace Rendering
{
    GLuint createProgram(const GLchar *vs, const GLchar *ps);
    GLuint createTexture(const char *filename);
    GLuint createVertexBuffer();
    GLuint createVertexBuffer(GLsizeiptr size, const void *data);

    void setTransform(Registry &registry, const float *matrix);
    void beginFrame(Registry &registry);
    void endFrame();

    void prepareForPC(Registry &registry);
    void drawLines(Registry &registry, const VertexPC *pVertices, int count);
    void drawPoints(Registry &registry, const VertexPC *pVertices, int count);
    void drawQuads(Registry &registry, const VertexPC *pVertices, int count);

    void prepareForPTC(Registry &registry);
    void drawLines(Registry &registry, const VertexPTC *pVertices, int count);
    void drawPoints(Registry &registry, const VertexPTC *pVertices, int count);
    void drawQuads(Registry &registry, const VertexPTC *pVertices, int count);

    int drawRect(VertexPTC *pVertices, float x, float y, float w, float h, const Color& color);
    int drawRect(VertexPC *pVertices, float x, float y, float w, float h, const Color& color);
    int drawLine(VertexPC *pVertices, const Position& from, const Position& to, const Color& color);
    int drawText(VertexPTC *pVertices, const std::string& text, const Position& position, const Color& bgColor, const Color& color, float align);
};

#endif
