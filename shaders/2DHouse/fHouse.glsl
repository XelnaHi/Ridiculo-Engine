#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main() {
    FragColor = texture(texture_diffuse1, TexCoord);
}
