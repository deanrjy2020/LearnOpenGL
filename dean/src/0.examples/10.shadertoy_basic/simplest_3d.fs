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
    uv += 0.5; // remap back to [0, 1]
    vec3 bottomColor = vec3(1.0, 0.0, 1.0);
    vec3 topColor = vec3(0.0, 1.0, 1.0);
    return mix(bottomColor, topColor, uv.y);
}

// return the distance from a point at p to the ray.
// 叉乘的size就是平行四边形的面积
float distLine(vec3 ro, vec3 rd, vec3 p)
{
    return length(cross(rd, p - ro)) / length(rd);
}

vec3 drawScene(vec2 uv)
{
    // background color by default
    vec3 color = getBackgroundColor(uv);

    // camera位置, 也就是ray origin
    vec3 ro = vec3(0., 0., 2.);
    // ray direction为从ro射向frag的射线(vector), 屏幕在z=0上, u就是x坐标, v就是y坐标.
    vec3 rd = normalize(vec3(uv, 0.) - ro);

    //vec3 p = vec3(0., 0., -3.);
    // p在y=0平面上移动
    vec3 p = vec3(sin(iTime), 0., cos(iTime));
    float d = distLine(ro, rd, p);

    // visualize the distance.
    // 中间的ray距离为0, 黑色, 旁边的距离变大, 变白
    //color = vec3(d);
    // 二分一下, edge=0.1,
    //color = vec3(step(.1, d));
    // sdf, 和背景混合
    color = mix(vec3(1., 1., 0.), color, step(.1, d));

    return color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy; // map to [0, 1]
    uv -= 0.5; // [-0.5,0.5]
    // 假设800*600 RT, 在fix aspect ratio后1就是600, x方向的600~800部分到不了了.
    uv.x *= iResolution.x / iResolution.y; // fix aspect ratio
    // 用下面这个fix后, 1就是800.
    //uv.y *= iResolution.y / iResolution.x;

    vec3 color = drawScene(uv);
    fragColor = vec4(color, 1.0);;
}

void main()
{
    mainImage(FragColor, gl_FragCoord.xy);
}