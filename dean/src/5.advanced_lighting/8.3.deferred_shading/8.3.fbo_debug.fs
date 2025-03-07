// fragment shader
#version 330 core
out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D fboAttachment;
uniform int displayMode;

void main()
{
    if (displayMode == 4) {
        // specular
        FragColor = vec4(texture(fboAttachment, TexCoords).a);
    } else if (displayMode == 3) {
        // albedo
        FragColor = vec4(texture(fboAttachment, TexCoords).rgb, 1.0);
    } else {
        FragColor = texture(fboAttachment, TexCoords);
    }
}