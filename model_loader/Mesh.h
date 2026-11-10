//
// Created by Teo on 2026-11-10.
//

#ifndef RIDICULOENGINE_MESH_H
#define RIDICULOENGINE_MESH_H
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "assimp/types.h"

class Shader;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int ID;
    std::string Type;
    aiString Path;
};


/*
 * Think of a mesh as smaller primitive pieces to draw, that together form the whole of something else. For example, the body of a human may consist of a head, shoulders, arms, a torso, and legs. These would be meshes, while the 'Model' would be all meshes put together.
 */
class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Shader& shader);

    private:
    // Render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


#endif //RIDICULOENGINE_MESH_H
