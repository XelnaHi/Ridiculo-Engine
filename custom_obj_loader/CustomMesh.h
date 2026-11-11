//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_CUSTOMMESH_H
#define RIDICULOENGINE_CUSTOMMESH_H
#include <vector>

#include "glm/glm.hpp"

struct CustomVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Shader;

class CustomMesh {
public:
    virtual void Render(Shader &aShader);

    virtual ~CustomMesh();

    void SetupMesh(std::vector<CustomVertex> vertices, std::vector<unsigned int> indices);
    glm::mat4 transform;

private:
    // Render data
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;
};


#endif //RIDICULOENGINE_CUSTOMMESH_H
