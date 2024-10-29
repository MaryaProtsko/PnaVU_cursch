uniform sampler2D texture;
uniform vec4 color;

void main()
{
    float pixel = texture2D(texture, gl_TexCoord[0].xy).x;
    gl_FragColor = color * pixel;
}