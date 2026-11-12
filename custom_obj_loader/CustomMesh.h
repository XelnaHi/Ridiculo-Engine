//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_CUSTOMMESH_H
#define RIDICULOENGINE_CUSTOMMESH_H
#include <string>
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

    void SetupMesh(const std::vector<CustomVertex> &vertices, const std::vector<unsigned int> &indices);

    std::string objModelName;

private:
    // Render data
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount = 0;
};


#endif //RIDICULOENGINE_CUSTOMMESH_H
