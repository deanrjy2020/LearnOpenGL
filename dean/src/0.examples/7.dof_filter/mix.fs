#version 330 core

precision highp float;

uniform sampler2D s_texClear;
uniform sampler2D s_texBlur;

uniform mat4 u_mat_yuv2rgb;

uniform vec2 u_center;
uniform vec2 u_distSquare;
uniform float u_fWHRatioSquare;
uniform float u_fMixWeight;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    vec4 clearSample = texture2D(s_texClear, TexCoords);
    vec4 blurSample = texture2D(s_texBlur, TexCoords);
#if 0
    // have figured out, see the FS of draw circle example.
    vec2 shiftCoord = TexCoords - u_center;
    float dist = shiftCoord.x * shiftCoord.x + shiftCoord.y * shiftCoord.y * u_fWHRatioSquare;
    float sw = smoothstep(u_distSquare.x, u_distSquare.y, dist);
    FragColor = mix(clearSample, blurSample, sw * u_fMixWeight);
#else
    // this is oval not circle due to the viewport ratio.
    vec2 shiftCoord = TexCoords - u_center;
    float radiusSQ = 0.25*0.25;
    float distSQ = shiftCoord.x * shiftCoord.x + shiftCoord.y * shiftCoord.y;
    if (distSQ < radiusSQ)
        FragColor = clearSample;
    else
        FragColor = blurSample;
#endif
}