#version 330 core
out vec4 FragColor;

in vec3 LightingColor;
// 这里应该也可以吧objectColor放到VS里面, 然后和LightingColor相乘在传过来.
// 区别就是给LightingColor做interpolation还是乘积做interpolation, 应该结果上没区别.
uniform vec3 objectColor;

void main()
{
   FragColor = vec4(LightingColor * objectColor, 1.0);
}