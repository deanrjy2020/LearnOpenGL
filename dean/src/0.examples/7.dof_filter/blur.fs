#version 330 core

precision highp float;

uniform sampler2D s_texture0;
uniform float u_fGaussian[15];
uniform vec2 u_fPixelShift[14];

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
//FragColor = texture2D(s_texture0, TexCoords);
//return;
	FragColor = u_fGaussian[0] * texture2D(s_texture0, TexCoords + u_fPixelShift[0]) +
				u_fGaussian[1] * texture2D(s_texture0, TexCoords + u_fPixelShift[1]) +
				u_fGaussian[2] * texture2D(s_texture0, TexCoords + u_fPixelShift[2]) +
				u_fGaussian[3] * texture2D(s_texture0, TexCoords + u_fPixelShift[3]) +
				u_fGaussian[4] * texture2D(s_texture0, TexCoords + u_fPixelShift[4]) +
				u_fGaussian[5] * texture2D(s_texture0, TexCoords + u_fPixelShift[5]) +
				u_fGaussian[6] * texture2D(s_texture0, TexCoords + u_fPixelShift[6]) +
				u_fGaussian[7] * texture2D(s_texture0, TexCoords) +
				u_fGaussian[8] * texture2D(s_texture0, TexCoords + u_fPixelShift[7]) +
				u_fGaussian[9] * texture2D(s_texture0, TexCoords + u_fPixelShift[8]) +
				u_fGaussian[10] * texture2D(s_texture0, TexCoords + u_fPixelShift[9]) +
				u_fGaussian[11] * texture2D(s_texture0, TexCoords + u_fPixelShift[10]) +
				u_fGaussian[12] * texture2D(s_texture0, TexCoords + u_fPixelShift[11]) +
				u_fGaussian[13] * texture2D(s_texture0, TexCoords + u_fPixelShift[12]) +
				u_fGaussian[14] * texture2D(s_texture0, TexCoords + u_fPixelShift[13]);
}