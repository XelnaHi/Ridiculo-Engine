#version 330 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main() {
    FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
}
