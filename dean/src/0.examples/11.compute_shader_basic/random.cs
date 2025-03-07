#version 430 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout (location = 0) uniform float t;                 /** Time */
layout (location = 1) uniform int frameIdx;

// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------

// seed.xy 是任意的 vec2, 可以传入 [0,1] 的 uv 坐标，也可以是更大的整数坐标（如 vec2(34.0, 92.0)）;
// 输出结果在 [0, 1) 区间
float random(vec2 seed) {
    return fract(sin(dot(seed.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// randSeed为(0, 0)开始的话, 一次可以, 多次后面都是0
vec2 randSeed;
float random() {
    randSeed += vec2(1.0, 1.0);  // 推进种子
    float rnd = random(randSeed);
    randSeed = vec2(rnd);
    return rnd;
}

void main() {
    // 把WG的number和size都看成是二维的, 每个WG里面的一个cube就是一个invocation, 对应一个pixel的地址,
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy); // [0, 999]
    ivec2 imgSz = imageSize(imgOutput);
    // 如果invocation超过了image size, 不用做.
    if (any(greaterThanEqual(texelCoord, imgSz))) {
        return;
    }

    vec2 uv = texelCoord * 1.0 / imgSz;
#if 0
    // 方法1, 用uv做种子
    // 缺点, 同一帧的同一个pixel, 多次调用, 生成的随机数是一样的, 不同帧的同一个pixel, 生成的随机数是也是一样的
    flat rnd = random(uv);
#elif 0
    // 方法2, uv和时间做种子, 部分解决上面的问题, 图像里没有明显的pattern
    // 缺点, 1, 不是deterministic, 每次运行都不一样, 因为时间是float的.
    // 2, 同一帧的同一个pixel, 多次调用, 生成的随机数是一样的, 因为用的是同一个时间.
    float rnd = random(t * uv);
#elif 0
    // 方法3, frameIdx和uv做种子, 解决了上面deterministic问题
    // 缺点, 1, 看图像明显有pattern
    // 2, 同一帧的同一个pixel, 多次调用, 生成的随机数是一样的
    float rnd = random(frameIdx * uv);
#else
    // 方法4, 第一次用uv做种子, 后面前一次的随机值做种子
    // from here: https://www.shadertoy.com/view/lssBD7
    //
    // 用下面这个方式模拟在一帧里面重复调用多次rand(), 看随机值的pattern, 好像还好, 即空间上没有明显的pattern.
    // 缺点, 时间上有pattern, 即从一个seed开始, 大概2000次后就重复了.
    if (frameIdx == 1) {
        randSeed = uv;
    } else {
        vec4 prePixel = imageLoad(imgOutput, texelCoord);
        if (prePixel.r != prePixel.g || prePixel.r != prePixel.b || prePixel.a != 1.0) {
            return;
        }
        randSeed = vec2(prePixel.r); // 读取上一帧随机值作为种子
    }
    float rnd = random();
#endif

    imageStore(imgOutput, texelCoord, vec4(vec3(rnd),1.0));
}