#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = 0.5 * texture(texture1, TexCoord) + 0.5 * vec4(ourColor, 1.0f);
}