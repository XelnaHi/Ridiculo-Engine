#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 TexCoord;
out vec3 Normal;

void main() {

    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;

    // Normals are originally defined as in model space, where as the directional light I intend to use is defined in world space.
    // The reason we need to apply transformation to the normal, "outside" of the model matrix calculations,
    // is because the model transformation is applied on the position attribute, and not on the normal. Without
    // normal transformations, the model would potentially rotate, but the normals would not, giving skewed values.

    // We're using a mat3 conversion of the model here, because we dont want to translate the normal, as the normal
    // is not a position vector, but a direction vector.

    // NOTE: using a mat3, scaling is also applied. This works fine when we're applying uniform scaling, but in case
    // we ever apply non-uniform scaling, we would have to do a transpose calculation, which I havent yet learned how
    // to.
    Normal = mat3(u_model) * aNormal;
}
