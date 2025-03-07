#version 330 core

// uncomment as needed.
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
// uniform float     iTimeDelta;            // render time (in seconds)
// uniform float     iFrameRate;            // shader frame rate
// uniform int       iFrame;                // shader playback frame
// uniform float     iChannelTime[4];       // channel playback time (in seconds)
// uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
// uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
// uniform vec4      iDate;                 // (year, month, day, time in seconds)
// uniform float     iSampleRate;           // sound sample rate (i.e., 44100)

out vec4 FragColor;

//insert WebGL code here

vec3 getBackgroundColor(vec2 uv)
{
    vec3 bottomColor = vec3(1.0, 0.0, 1.0);
    vec3 topColor = vec3(0.0, 1.0, 1.0);
    return mix(bottomColor, topColor, uv.y);
}

// translate = offset
// return <0, 如果位置在circle里面, >0, 位置在circle外面
float inCircle(vec2 uv, float r, vec2 translate)
{
    // 在(0.5, 0.5)的基础上向上/右移动一点是-offset, 不是+
    uv -= translate;
    return length(uv) - r;
}

// R = | cos(theta) -sin(theta) |
//     | sin(theta)  cos(theta) |
// column first
// after multiply, each frag is rotate clock-wise.
vec2 rotate(vec2 uv, float theta)
{
    return mat2(cos(theta), sin(theta), -sin(theta), cos(theta)) * uv;
}

// 和3D一样, 反着来, 先rotate, 然后平移到translate位置
// return <0, 里面, >0, 外面
float inSquare(vec2 uv, float r, vec2 translate)
{
    uv -= translate;
    uv = rotate(uv, iTime);
    return max(abs(uv.x), abs(uv.y)) - r;
}

// 就是一个函数, 不用记, 理解就好, 和圆是一样原理
// (x^2 + y^2 - 1)^3 - x^2*y^3 = 0; 其中1就是heart的大小.
float inHeart(vec2 uv, float size, vec2 translate)
{
    uv -= translate;
    float uu = uv.x * uv.x;
    float vv = uv.y * uv.y;
    float vvv = vv * uv.y;
    float val = uu + vv - size;
    float val_3 = val * val * val;
    return val_3 - uu * vvv;
}

// 五角星, 不用理解
float inStar5(in vec2 p, in float r, in float rf, vec2 offset)
{
    p -= offset; // This will subtract offset.x from p.x and subtract offset.y from p.y
    const vec2 k1 = vec2(0.809016994375, -0.587785252292);
    const vec2 k2 = vec2(-k1.x,k1.y);
    p.x = abs(p.x);
    p -= 2.0*max(dot(k1,p),0.0)*k1;
    p -= 2.0*max(dot(k2,p),0.0)*k2;
    p.x = abs(p.x);
    p.y -= r;
    vec2 ba = rf*vec2(-k1.y,k1.x) - vec2(0,1);
    float h = clamp( dot(p,ba)/dot(ba,ba), 0.0, r );
    return length(p-ba*h) * sign(p.y*ba.x-p.x*ba.y);
}

// 矩形, 不用理解
float inBox( in vec2 p, in vec2 b, vec2 offset )
{
    p -= offset;
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

vec3 drawScene(vec2 uv)
{
    // background color by default
    vec3 color = getBackgroundColor(uv);

    //vec2 offset = vec2(sin(iTime*2.)*0.2, cos(iTime*2.)*0.2); // move the circle clockwise

    // yellow circle
    float circle = inCircle(uv, 0.1, vec2(0.15, 0.5));
    // 在mix后可以理解为把circle的颜色混合进了背景颜色.
    color = mix(vec3(1.0, 1.0, 0.0), color, step(0.0, circle));

    // red square
    float square = inSquare(uv, 0.1, vec2(0.4, 0.5));
    color = mix(vec3(1.0, 0.0, 0.0), color, step(0.0, square));

    // green heart
    float heart = inHeart(uv, 0.04, vec2(0.8, 0.5));
    color = mix(vec3(0.0, 1.0, 0.0), color, step(0.0, heart));

    // yellow star5
    float star5 = inStar5(uv, 0.12, 0.45, vec2(0.15, 0.2));
    color = mix(vec3(1.0, 1.0, 0.0), color, step(0.0, star5));

    // red rect
    float box = inBox(uv, vec2(0.2, 0.1), vec2(0.5, 0.2));
    color = mix(vec3(1.0, 0.0, 0.0), color, step(0.0, box));

    return color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy; // map to [0, 1]
    // 假设800*600 RT, 在fix aspect ratio后1就是600, x方向的600~800部分到不了了.
    uv.x *= iResolution.x / iResolution.y; // fix aspect ratio
    // 用下面这个fix后, 1就是800.
    //uv.y *= iResolution.y / iResolution.x;

    vec3 color = drawScene(uv);
    fragColor = vec4(color, 1.0);
}

void main()
{
    mainImage(FragColor, gl_FragCoord.xy);
}