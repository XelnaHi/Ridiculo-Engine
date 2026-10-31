#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float u_xOffset;
uniform float u_yOffset;

uniform int u_xInvert;
uniform int u_yInvert;
uniform int u_invert;

uniform float u_xScale;
uniform float u_yScale;

uniform float u_translateY;
uniform float u_translateX;

out vec3 roofColor;

void main() {

    float xScale = aPos.x * u_xScale;
    float yScale = aPos.y * u_yScale;

    xScale *= u_invert;
    yScale *= u_invert;

    float xPos = xScale + u_xOffset;
    float yPos = yScale + u_yOffset;

    float finalX = xPos + u_translateX;
    float finalY = yPos + u_translateY;

    vec3 scaledPos = vec3(finalX, finalY, aPos.z);

    gl_Position = vec4(scaledPos, 1.0);
    roofColor = aColor;
}
