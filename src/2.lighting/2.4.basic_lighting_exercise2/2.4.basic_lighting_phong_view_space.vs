#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // do the light pos view space in the VS (once per vertex) and pass to FS.
    // We can also do on CPU (only once per draw) and pass to FS.
    LightPos = vec3(view * model * vec4(lightPos, 1.0));
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    gl_Position = projection * vec4(FragPos, 1.0);
}