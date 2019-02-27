uniform mat4 ProjMtx;

attribute vec2 Position;
attribute vec4 Color;

varying vec4 Frag_Color;

void main()
{
    Frag_Color = Color;
    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
}
