//
// Created by Teo on 2026-11-10.
//

#include "Mesh.h"

#include "glad/glad.h"
#include "../Shader.h"

/*
 * Taking the parameters by value and moving them into the members avoids a second copy.
 *
 * Inside the constructor the parameters are lvalues: they are named and can be
 * referred to (and have their address taken) throughout the constructor's scope.
 * rvalues, by contrast, are unnamed temporaries such as literals or function
 * return values.
 *
 * std::move casts each lvalue to an rvalue, which selects the vector's move
 * constructor for the member being initialized. That constructor takes over the
 * parameter's heap buffer pointer instead of allocating and copying the elements,
 * leaving the parameter empty.
 */
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : vertices(std::move(vertices)),
      indices(std::move(indices)),
      textures(std::move(textures)) {
    setupMesh();
}

/*
 * The draw(..) function operates on the assumption that sampler uniforms follow a strict naming convention;
 * - texture_diffuseN
 * - texture_specularN
 *
 * Examples;
    uniform sampler2D texture_diffuse1;
    uniform sampler2D texture_diffuse2;
    uniform sampler2D texture_diffuse3;
    uniform sampler2D texture_specular1;
    uniform sampler2D texture_specular2;
 */
void Mesh::draw(Shader &shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;

    for (int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i); // Activate the correct texture unit before binding.

        std::string number;
        std::string name = textures[i].Type;

        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
            // the (value++) operator doesn't actually increment the value until the execution has finished, meaning we will fetch the element at index 0 (on first iteration), and will only update the number value to be = 1 after the assignment has finished. Next iteration, we will fetch the element at index 1, and update the number to 2 after assignment has completed.
        } else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
        } else if (name == "texture_normal") {
            number = std::to_string(normalNr++);
        }

        // Apparently string concatenation casts the result to a c-style string.
        shader.setInt(name + number, i);

        // Here we specify which texture handle goes into the previously activated texture unit. I gather that the reason we have to do this for textures, but not for, say, VBO's, is that which VBO is bound is a global state limited to a single "slot", while textures can be bound to 1 of at least 16 texture slots (depends on the driver).
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    /*
     *My understanding is that the VAO is an internal driver data structure that stores configurations like the ones below;
     * - Which VBO per attribute to read from. That association is captured at the time of calling glVertexAttribPointer(..) and depends on which VBO is currently bound at the time.
     * - The EBO binding. Association is created same way as for VBO's. However, the VAO only stores a reference to the EBO, not the actual data pertaining to the EBO itself.
     * - The data belonging to the VBO and EBO is stored on the GPU.
    */
    glGenVertexArrays(1, &VAO); // Creates the VAO handle on CPU
    glGenBuffers(1, &VBO); // Creates the VBO handle on CPU
    glGenBuffers(1, &EBO); // Creates the EBO handle and reference

    glBindVertexArray(VAO); // Set which VAO is currently selected
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds a VBO to the currently selected VAO

    /* Question: Were passing a reference to the first index of the vertices vector. Why? Previously I have only supplied the data as a c-style array.
     * Answer: because the function expects a pointer to the first element of contiguous memory. Using a c-style array, passing the array itself decays it into a pointer
     * which is not equally true for vector types. If I supplied only the vertices vector, it would be like passing a pointer to the vector object itself, not it'
     * contents. Passing a reference to the vector objects first element ensure we pass the starting point of the vectors heap buffer.
     *
     * Additionally, it's worth remembering that structs byte data is allocated in memory contiguously, same as c-style arrays and vector heap buffers, which is what allows us to pass a reference to the pointer of the first vertices element. I assume that this is efficient because passing a single pointer is very efficient as opposed to potentially dropping a very large list of vertices into the buffer which would probably have to do it's own calculations as to where the byte data begins.
    */
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

    // Vertex normals
    glEnableVertexAttribArray(1);
    // NOTE: 'offsetof' is a preprocessor macro (signature: offsetof(s,m)), that takes a struct as it's first argument and a struct member name as it's second. The macro returns the byte offset of that variable from the start of the struct. Very handy when wanting to configure the byte offset for the GPU.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

    // Vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
