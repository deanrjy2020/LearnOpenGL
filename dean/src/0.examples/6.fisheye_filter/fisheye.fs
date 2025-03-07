#version 330 core
#if 0
precision mediump float;
uniform vec2 u_distort;
uniform vec2 u_shift;
uniform vec2 u_scale;
uniform vec2 u_center;

uniform sampler2D s_texture0;
uniform mat4 u_mat_yuv2rgb;

in vec2 TexCoords;
out vec4 FragColor;

float stx,sty,sta,stb,a,b,rr,r,nr,nx2,ny2,minr,maxr,f,pf;

void main () {
    vec2 texpos = TexCoords*u_scale-u_shift;
    stx = texpos.x* texpos.x;
    sty = texpos.y * texpos.y;
    rr =sqrt(stx+sty);
    a = -sign(texpos.x)*u_center.x+ceil(texpos.x);
    b = -sign(texpos.y)*u_center.y+ceil(texpos.y);
    sta = a*a; // 1
    stb = b*b; // 1
    ny2 =  (sta*stb)/(stx*stb/sty+sta) ;
    nx2 = stx*ny2/sty;
    nr= sqrt(nx2+ny2);
    minr = nr*exp(log((rr)/(nr))*u_distort.x);
    f = u_distort.x - 1.0;
    pf= 2.0/(1.0+pow(max(0.0,(rr-0.7)),1.6)*15.0);
    r=rr*rr*pf*f;
    maxr = rr*(1.0+r*1.5+r*rr*rr)*(1.0-pow(f,0.5)*0.72);
    r = (ceil(f)*maxr + (1.0-ceil(f))*minr)/rr;
#if 0
    FragColor = u_mat_yuv2rgb * texture2D(s_texture0, (texpos*r+u_center));
#else
    FragColor = texture2D(s_texture0, texpos);
#endif
}
#else

            precision mediump float;
            uniform sampler2D tex_sampler_0;
            uniform vec2 scale;
            uniform float alpha;
            uniform float radius2;
            uniform float factor;
            //varying vec2 v_texcoord;
            in vec2 TexCoords;
            void main() {
              const float m_pi_2 = 1.570963;
              vec2 coord = TexCoords - vec2(0.5, 0.5);
              float dist = length(coord * scale);
              float radian = m_pi_2 - atan(alpha * sqrt(radius2 - dist * dist), dist);
              float scalar = radian * factor / dist;
              vec2 new_coord = coord * scalar + vec2(0.5, 0.5);
              gl_FragColor = texture2D(tex_sampler_0, new_coord);
            }


#endif