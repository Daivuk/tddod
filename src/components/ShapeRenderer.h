/*
    Draw a shape at Position with Color.
*/

#ifndef SHAPERENDERER_H_INCLUDED
#define SHAPERENDERER_H_INCLUDED

struct Color;
struct Position;
struct VertexPC;

typedef int(*DRAW_SHAPE_FN)(VertexPC *, const Position &, float, float, const Color &);

struct ShapeRenderer
{
    DRAW_SHAPE_FN drawShape;
};

#endif
