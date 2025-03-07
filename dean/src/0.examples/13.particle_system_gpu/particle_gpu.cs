#version 430

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle {
    vec4 pos;
    vec4 vel;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform float dt = 0.01;

void main() {
    uint id = gl_GlobalInvocationID.x;
    if (id < 1024) {
        // 计算和写入数据

    particles[id].pos.xyz += particles[id].vel.xyz * dt;

    /*
            // 获取粒子数据
        vec4 pos = particles[id].xyz;
        vec4 velocity = particles[id].xyz;

        // 更新粒子位置
        pos += velocity * deltaTime;

        // 简单的重置粒子（超出屏幕范围）
        if (pos.x > 1.0 || pos.x < -1.0 || pos.y > 1.0 || pos.y < -1.0) {
            pos = vec4(0.0, 0.0, 0.0, 1.0); // 位置重置
        }

        // 更新粒子数据
        particles[id] = pos;
    */
    }
}
