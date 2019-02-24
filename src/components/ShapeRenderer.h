#ifndef SHAPERENDERER_H_INCLUDED
#define SHAPERENDERER_H_INCLUDED

typedef int(*DRAW_SHAPE_FN)(VertexPC *, const Position &, float, float, const Color &);

struct ShapeRenderer
{
    DRAW_SHAPE_FN drawShape;
};

#endif
