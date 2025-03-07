#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout (location = 0) uniform float t;                 /** Time */

// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------
struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
};

struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

void main() {
#if 1
    vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
    // 把WG的number和size都看成是二维的, 每个WG里面的一个cube就是一个invocation, 对应一个pixel的地址,
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy); // [0, 999]
    ivec2 size = imageSize(imgOutput);

    // 如果invocation超过了image size, 不用做.
    if (any(greaterThanEqual(texelCoord, size))) {
        return;
    }


    float speed = 100;
    // the width of the texture
    float width = 1000;

    value.x = mod(float(texelCoord.x) + t * speed, width) / (gl_NumWorkGroups.x * gl_WorkGroupSize.x);
    value.y = float(texelCoord.y)/(gl_NumWorkGroups.y*gl_WorkGroupSize.y);
    // 如果不考虑时间变化的话, 上面的两行和下面的是一样的效果.
    //value.xy = 1. * texelCoord / size; // map to [0, 1]

    imageStore(imgOutput, texelCoord, value);
#else

    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screen_size = imageSize(imgOutput);
    vec2 coeff = (pixel_coords * 2. - screen_size) / screen_size; // map to [-1, 1]

    vec3 pixel = vec3(0.0);

    Camera camera;
    camera.position = vec3(0.0);
    camera.front = vec3(0.0, 0.0, -1.0);
    camera.up = vec3(0.0, 1.0, 0.0);
    camera.right = cross(camera.front, camera.up);//vec3(1.0, 0.0, 0.0);

    Sphere sphere;
    sphere.center = vec3(0.0, 0.0, -3.0);
    sphere.radius = 1.0;
    sphere.color = vec3(1.0, 0.3, 0.7);

    Ray ray;
    ray.origin = camera.position;
    // 由coeff为[-1, 1]可知,
    // 如果是image中间点, rd = camera.front + 0 + 0
    // 如果是中间点的最左边, rd = camera.front + -1 * camera.right + 0
    // 如果是中间点的最右边, rd = camera.front +  1 * camera.right + 0
    // 线上的其他点就是均分, 或者插值.
    // y方向上的同理.
    ray.direction = normalize(camera.front + coeff.x * camera.right + coeff.y * camera.up);

    // 一元二次方程求解
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(ray.direction, ray.origin - sphere.center);
    float c = dot(ray.origin - sphere.center, ray.origin - sphere.center) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0) {
        pixel += sphere.color;
    }

    imageStore(imgOutput, pixel_coords, vec4(pixel,1.0));

#endif
}