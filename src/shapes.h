#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

struct Color;
struct Position;
struct VertexPC;

typedef int(*DRAW_SHAPE_FN)(VertexPC *, const Position&, float, float, const Color&);

int drawBox(VertexPC *pVertices, const Position& position, float w, float h, const Color& color);
int drawCircle(VertexPC *pVertices, const Position& position, float w, float h, const Color& color);

#endif
