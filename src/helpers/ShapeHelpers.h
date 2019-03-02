/*
    Shape drawing helpers
*/

#ifndef SHAPEHELPERS_H_INCLUDED
#define SHAPEHELPERS_H_INCLUDED

struct Color;
struct Position;
struct VertexPC;

namespace Shape
{
    /*
        The following functions doesn't render on the screen, but generates
        vertices in pVertices and returns the number of vertices added.
    */
    int drawBox(VertexPC *pVertices, const Position &position, float w, float h, const Color &color);
    int drawCircle(VertexPC *pVertices, const Position &position, float w, float h, const Color &color);
}

#endif
