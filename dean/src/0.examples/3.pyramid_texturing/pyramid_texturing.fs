#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    // when reading the tex from file, the origin is left-top,
    // revert to left-bottom to keep the same as OpenGL.
    FragColor = texture(texture1, vec2(TexCoords.x, 1.0 - TexCoords.y));
}