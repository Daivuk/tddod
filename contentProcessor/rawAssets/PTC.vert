uniform mat4 ProjMtx;

attribute vec2 Position;
attribute vec2 TexCoord;
attribute vec4 Color;

varying vec2 Frag_TexCoord;
varying vec4 Frag_Color;

void main()
{
    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
    Frag_TexCoord = TexCoord;
    Frag_Color = Color;
}
