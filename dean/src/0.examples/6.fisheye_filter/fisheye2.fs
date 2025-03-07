#version 330 core

 //varying vec2 TexCoords;
 in vec2 TexCoords;

 uniform sampler2D inputImageTexture;

 uniform float aspectRatio;
 uniform vec2 center;
 uniform float radius;
 uniform float scale;

 void main()
 {
    vec2 TexCoordsToUse = vec2(TexCoords.x, ((TexCoords.y - center.y) * aspectRatio) + center.y);
    float dist = distance(center, TexCoordsToUse);
    TexCoordsToUse = TexCoords;

    if (dist < radius)
    {
        TexCoordsToUse -= center;
        float percent = 1.0 - ((radius - dist) / radius) * scale;
        percent = percent * percent;

        TexCoordsToUse = TexCoordsToUse * percent;
        TexCoordsToUse += center;
    }

    gl_FragColor = texture2D(inputImageTexture, TexCoordsToUse );
 }


            // precision mediump float;
            // uniform sampler2D tex_sampler_0;
            // uniform vec2 scale;
            // uniform float alpha;
            // uniform float radius2;
            // uniform float factor;
            // //varying vec2 v_texcoord;
            // in vec2 TexCoords;
            // void main() {
            //   const float m_pi_2 = 1.570963;
            //   vec2 coord = TexCoords - vec2(0.5, 0.5);
            //   float dist = length(coord * scale);
            //   float radian = m_pi_2 - atan(alpha * sqrt(radius2 - dist * dist), dist);
            //   float scalar = radian * factor / dist;
            //   vec2 new_coord = coord * scalar + vec2(0.5, 0.5);
            //   gl_FragColor = texture2D(tex_sampler_0, new_coord);
            // }
