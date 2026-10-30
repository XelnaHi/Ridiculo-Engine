#version 330 core

in vec3 roofColor;
out vec4 FragColor;

uniform vec4 u_Color;

void main() {
    FragColor = vec4(u_Color);
}
