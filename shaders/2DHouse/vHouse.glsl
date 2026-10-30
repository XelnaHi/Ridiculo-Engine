#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float u_xOffset;
uniform float u_yOffset;

uniform int u_xInvert;
uniform int u_yInvert;

uniform float u_xScale;
uniform float u_yScale;

out vec3 roofColor;

void main() {
    float xPos = (aPos.x + u_xOffset) * u_xInvert;
    float yPos = (aPos.y + u_yOffset) * u_yInvert;

    xPos *= u_xScale;
    yPos *= u_yScale;

    vec3 scaledPos = vec3(xPos, yPos, aPos.z);

    gl_Position = vec4(scaledPos, 1.0);
    roofColor = aColor;
}
