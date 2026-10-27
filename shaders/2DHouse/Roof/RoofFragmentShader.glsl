#version 330 core

layout (location = 0) in vec3 aPos;

in vec3 roofColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(roofColor, 1.0);
}
