#ifndef SHAPEHELPERS_H_INCLUDED
#define SHAPEHELPERS_H_INCLUDED

struct Color;
struct Position;
struct VertexPC;

namespace Shape
{
    int drawBox(VertexPC *pVertices, const Position& position, float w, float h, const Color& color);
    int drawCircle(VertexPC *pVertices, const Position& position, float w, float h, const Color& color);
}

#endif
