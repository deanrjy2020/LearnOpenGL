#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
#if 1
    // access the neighboring texels by using the textureOffset()
    FragColor = vec4(0.0);
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1, 1)); // top-left
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 0, 1)); // top-center
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1, 1)); // top-right
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1, 0)); // center-left
    // 这是Laplacian算子, 中间应该是8我不是9?
    FragColor +=  9 * textureOffset(screenTexture, TexCoords, ivec2( 0, 0)); // center-center
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1, 0)); // center-right
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1,-1)); // bottom-left
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 0,-1)); // bottom-center
    FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1,-1)); // bottom-right
#else
    // bypass, for debugging
    FragColor = texture(screenTexture, TexCoords);
#endif
} 