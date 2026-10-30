#version 330 core

in vec3 roofColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(roofColor, 1.0);
}
