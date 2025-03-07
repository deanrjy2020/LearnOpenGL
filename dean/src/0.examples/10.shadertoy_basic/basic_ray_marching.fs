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

// https://www.youtube.com/watch?v=PGtv-dBi2wE
// https://www.youtube.com/watch?v=Ff0jJyyiVyw
// https://www.shadertoy.com/view/4dSfRc
// https://inspirnathan.com/posts/52-shadertoy-tutorial-part-6
// https://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/

#define MAX_STEPS 100
#define MAX_MARCH_DIST 100.
#define HIT_DIST .01

// 简单的plane, 只有y值
float planeDist(vec3 p, float plane)
{
    return p.y - plane;
}

// r为半径, 圆心为原点, 即sphere的local space
float sdSphere(vec3 p, float r)
{
    return length(p) - r;
}

// 这个是直接在world space下做的.
// p是空间中的一点, Capsule由两个圆心和半径定义.
// 返回p到capsule的距离
float sdCapsule(vec3 p, vec3 a, vec3 b, float r)
{
    vec3 ab = b-a;
    vec3 ap = p-a;
    // t == |ap| * cos_theta / |ab|
    // c为p的垂直点, t为|ac| / |ab|
    float t = dot(ap, ab) / dot(ab, ab);
    // c可能不在ab中间, 而在两端, 如果在a这端, 就用a(即t=0), 如果在b这端就用b(即t=1)
    t = clamp(t, 0., 1.);
    // 算出c坐标
    vec3 c = a + t*(b-a);

    return length(p-c) - r;
}

// torus有点不一样, 其他的直接在world space下做 (也可以在local space下做),
// torus在local space做方便点, 定义在local space的y=0面上,
// 圆心为原点, 由两个半径定义.
// p为torus local space下的坐标
float sdTorus(vec3 p, vec2 r)
{
    // length(p.xz)为p在y=0平面上的投影p'到圆心的距离
    // 减去大半径r.x后为p'到圆环中心的距离
    // 问题, 如果p在其上面, 投影p'到圆心的距离会比大半径小会不会出问题?
    float x = length(p.xz) - r.x;
    // p到圆环中心的距离
    float d = length(vec2(x, p.y));
    // 减去小半径后为p到圆环的距离
    return d - r.y;
}

// local space, 用二维的推导的, 不用记.
// https://iquilezles.org/articles/distfunctions/
float sdBox(vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float getSceneDist(vec3 p)
{
    float pd = planeDist(p, 0.);

    vec3 spherePos = vec3(0., 1., 6.);
    float sd = sdSphere(p - spherePos, .7);
    // 为什么多个obj都是用min放到scene里面?
    float d = min(pd, sd);

    float cd = sdCapsule(p, vec3(1.5, 1, 6), vec3(2.5, 2, 6), .2);
    d = min(cd, d);

    // 将p移动到torus的local space,
    // 如果是移动torus到world space, 就是torus的圆心(0, 0, 0)+torusPos
    // 移动p就是反向移动torusPos
    vec3 torusPos = vec3(-2.5, .5, 6.);
    float td = sdTorus(p - torusPos, vec2(.7, .3));
    d = min(td, d);

    vec3 boxPos = vec3(4., .5, 6.);
    float db = sdBox(p - boxPos, vec3(.5, .5, .5));
    d = min(db, d);

    return d;
}

// ray marching核心算法
float rayMarch(vec3 ro, vec3 rd)
{
    // distance from the origin, do is a keyword, use d_o.
    float d_o = 0.;
    // 最多走100步
    for (int i = 0; i< MAX_STEPS; ++i) {
        // 从origin开始, 在rd方向上前进do长度, called marching
        vec3 p = ro + d_o * rd;
        // 前进后的位置和物体的距离
        // 注意这里不是单个物体, 而是整个scene里面的物体.
        float ds = getSceneDist(p);
        // 第一次得到的ds就是1, d_o一直在累加, 最后的d_o结果>1
        d_o += ds;
        // hit到物体了, 或者100步到了, 都停止走
        if (ds < HIT_DIST || d_o > MAX_MARCH_DIST) {
            break;
        }
    }
    return d_o;
}

// p为frag world space坐标
// 这个也没有讲清楚.
vec3 getNormal(vec3 p)
{
    float d = getSceneDist(p);
    vec2 e = vec2(.01, 0.);

    vec3 n = d - vec3(
        getSceneDist(p - e.xyy),
        getSceneDist(p - e.yxy),
        getSceneDist(p - e.yyx));
    return normalize(n);
}

// p是物体上的点, 即frag在3d world space的坐标
float getLight(vec3 p)
{
    // 光源在球正上方, 和球距离6-2*1 = 4.
    vec3 lightPos = vec3(0, 5, 6);
    // 动起来
    lightPos.xz += vec2(sin(iTime), cos(iTime)) * 2.;


    vec3 l = normalize(lightPos - p);
    vec3 n = getNormal(p);

    float diff = clamp(dot(l, n), 0., 1.);

    // 计算shadow
    // 从当前点到light做ray marching,
    // 如果结果距离是小于当前点到light距离,说明中间有东西, 就是在shadow里面
    // 拿palne上的p为例, 因为p就在plane上, 做ray marching马上return了,
    // (认为自己就是中间的物体), 所以要往normal的方向移动一点
    // 如果只移动HIT_DIST, 球面上朝向camera的点还是有问题, 所以要比HIT_DIST多一点.
    float d = rayMarch(p + n * HIT_DIST * 2., l);
    if (d < length(p-lightPos)) {
        diff *= .1;
    }
    return diff;
}

// 这里全部是world space坐标
vec3 drawScene(vec2 uv)
{
    // camera在原点上面y=2位置上.
    vec3 ro = vec3(0, 2, 0);
    // ? 这个是如何定义出来的, 视频没有将清楚, 不应该是先定义屏幕的世界坐标系位置p1,
    // 然后每个rd = p1 - ro, 即从camera发射出来, 射向每个pixel?
    vec3 rd = normalize(vec3(uv.x, uv.y-.2, 1));

    // d是ray和物体相交的距离, 一个frag对应一个ray (上面的ro, rd)
    float d = rayMarch(ro, rd);

    vec3 p = ro + d * rd;

    // visualize the normals for all point/frag in world space
    //color = getNormal(p);

    vec3 color = vec3(getLight(p));

    return color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy; // map to [0, 1]
    uv -= .5;
    uv.x *= iResolution.x / iResolution.y;

    vec3 color = drawScene(uv);
    fragColor = vec4(color, 1.0);
}

void main()
{
    mainImage(FragColor, gl_FragCoord.xy);
}