//
// Created by Teo on 2026-11-11.
//

#include "CustomMesh.h"

#include "Renderer.h"
#include "../Shader.h"
#include "glad/glad.h"

void CustomMesh::Render() {
    Renderer::drawCalls++;

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

CustomMesh::~CustomMesh() {
}

/*
 * Called from the OBJLoader
 */
void CustomMesh::SetupMesh(const std::vector<CustomVertex> &vertices, const std::vector<unsigned int> &indices) {

    indexCount = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CustomVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CustomVertex), (void*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CustomVertex), (void*)offsetof(CustomVertex, normal));

    // TexCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CustomVertex), (void*)offsetof(CustomVertex, uv));
}
