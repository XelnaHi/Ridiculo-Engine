#version 330 core

in vec2 TexCoord;
in vec3 Normal;

uniform vec3 lightDirection;
uniform vec3 lightColor;

out vec4 FragColor;


void main() {

    // The normal needs to be normalized as well, as the defined normals are only defined per vertex. The problem that
    // arise if we dont normalize the normal in the fragment shader is that the normal attribute for each vertex will be
    // blended (interpolated) together with all the other defined vertex normals, with different strength
    // (barycentric weight) depending on where the current fragment is within the primitve.
    vec3 norm = normalize(Normal);


    // Get the dot product between two normalized vectors in order to calculate if the directional light is
    // directly above the fragment (which would give a higher color strength) or if it's perpendicular to the fragment
    // at which point the directional light would have no effect on the fragment.
    //
    // Also, we need to invert the light direction, because the dot product assumes that both vectors point away from
    // the same surface. Originally, the light direction points towards the face, but the primitive points away from
    // the face. Performing DOT product on these types of vectors would not give us proper results.
    //
    // For example, if we had a directional light that pointed directly towards a surface, but we didn't invert the
    // directional light, we would get this kind of calculation:
    // dot( normal (0,1,0), lightDirection (0,-1,0) ) = -1
    // Since we're clamping in our differentials calculation, that value would get clamped to 0, which would translate
    // make the object completely dark, as if no light. Inverting the light dir first would instead give us the value: 1.
    vec3 lightDir = normalize(-lightDirection);

    // Negative color values are not defined, so we clamp the value between 0, and since we are operating on normalized
    // vectors, we can assume the potential max value is 1.

    // We're doing the above calculation on the Normal vector and the Directional Lights direction vector,
    // because the normal vector represents each fragments "front" direction within a face.
    float diff = max(dot(norm, lightDir), 0.0f);

    // This calculation will give us the object's reflected value.
    vec3 diffuse = diff * lightColor;
    vec3 ambient = 0.15f * lightColor;

    FragColor = vec4(ambient + diffuse, 1.0);

}
