#version 330 core
out vec4 FragColor;
uniform float alpha;

void main() {
    FragColor = vec4(1.0, 0.5, 0.1, alpha);
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
