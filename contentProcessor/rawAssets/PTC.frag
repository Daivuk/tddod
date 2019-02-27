varying vec2 Frag_TexCoord;
varying vec4 Frag_Color;

uniform sampler2D Texture;

void main()
{
    vec4 diffuse = texture2D(Texture, Frag_TexCoord);
    gl_FragColor = diffuse * Frag_Color;
}
