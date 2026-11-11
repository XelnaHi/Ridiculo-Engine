#version 330 core

layout (location = 0) in vec3 aPos;
// TODO: add layout location for normals which is set in 'Mesh::setupMesh(..)
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 TexCoord;

void main() {

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
