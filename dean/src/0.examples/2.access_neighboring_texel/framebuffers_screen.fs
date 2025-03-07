#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int method;

void main()
{

    // 效果应该是一样的.
    if (method == 1) {
        // access the neighboring texels by using the textureOffset()
        FragColor = vec4(0.0);
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1, 1)); // top-left
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 0, 1)); // top-center
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1, 1)); // top-right
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1, 0)); // center-left
        FragColor +=  9 * textureOffset(screenTexture, TexCoords, ivec2( 0, 0)); // center-center
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1, 0)); // center-right
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2(-1,-1)); // bottom-left
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 0,-1)); // bottom-center
        FragColor += -1 * textureOffset(screenTexture, TexCoords, ivec2( 1,-1)); // bottom-right
    } else if (method == 2) {
        // 用textureSize得到texture的大小, 算得每个texel的大小, 直接用texture()去采样.
        vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
        FragColor = vec4(0.0);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2(-1, 1) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2( 0, 1) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2( 1, 1) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2(-1, 0) * texelSize);
        FragColor +=  9 * texture(screenTexture, TexCoords + ivec2( 0, 0) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2( 1, 0) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2(-1,-1) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2( 0,-1) * texelSize);
        FragColor += -1 * texture(screenTexture, TexCoords + ivec2( 1,-1) * texelSize);
    } else {
        // bypass, for debugging
        FragColor = texture(screenTexture, TexCoords);
    }
}