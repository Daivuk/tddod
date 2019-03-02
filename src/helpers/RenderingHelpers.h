/*
    Helper functions to abstract opengl calls
*/

#ifndef RENDERINGHELPERS_H_INCLUDED
#define RENDERINGHELPERS_H_INCLUDED

#include <GL/gl3w.h>

#include <vector>

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
    /*
        Create a shader program from vertex/fragment shader source.
    */
    GLuint createProgram(const GLchar *vs, const GLchar *ps, const std::vector<const char *> &attribs);

    /*
        Load a texture from an image file
    */
    GLuint createTexture(const char *filename);

    /*
        Returns an empty vertex buffer
    */
    GLuint createVertexBuffer();

    /*
        Returns a static vertex buffer
    */
    GLuint createVertexBuffer(GLsizeiptr size, const void *data);

    /*
        Set a new viewProjection matrix into all of the Resources singleton 
        component's shaders.
    */
    void setTransform(Registry &registry, const float *matrix);

    /*
        Start a rendering frame. This will clear background to black and
        set defaults render states.
    */
    void beginFrame(Registry &registry);

    /*
        This does nothing, but might eventually. Mirror it with beginFrame.
    */
    void endFrame();

    /*
        Prepare the rendering pipeline for Position + Color rendering.
    */
    void prepareForPC(Registry &registry);
    void drawLines(Registry &registry, const VertexPC *pVertices, int count);
    void drawPoints(Registry &registry, const VertexPC *pVertices, int count);
    void drawQuads(Registry &registry, const VertexPC *pVertices, int count);

    /*
        Prepare the rendering pipeline for Position + Texture + Color rendering.
    */
    void prepareForPTC(Registry &registry);
    void drawLines(Registry &registry, const VertexPTC *pVertices, int count);
    void drawPoints(Registry &registry, const VertexPTC *pVertices, int count);
    void drawQuads(Registry &registry, const VertexPTC *pVertices, int count);

    /*
        The following functions doesn't render on the screen, but generates
        vertices in pVertices and returns the number of vertices added.
    */
    int drawRect(VertexPTC *pVertices, float x, float y, float w, float h, const Color &color);
    int drawRect(VertexPC *pVertices, float x, float y, float w, float h, const Color &color);
    int drawLine(VertexPC *pVertices, const Position &from, const Position &to, const Color &color);
    int drawText(VertexPTC *pVertices, const std::string &text, const Position &position, const Color &bgColor, const Color &color, float align);
};

#endif
