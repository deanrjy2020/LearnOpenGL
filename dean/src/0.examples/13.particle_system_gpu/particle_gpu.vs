#version 430

//layout(location = 0) in vec4 inPosition;

struct Particle {
    vec4 pos;
    vec4 vel;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

void main() {
    vec3 pos = particles[gl_VertexID].pos.xyz;
    gl_Position = vec4(pos, 1.0);
    gl_PointSize = 2.0;
}
