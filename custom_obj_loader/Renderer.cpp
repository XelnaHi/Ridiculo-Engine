//
// Created by Teo on 2026-11-11.
//

#include "Renderer.h"

#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "CustomMesh.h"
#include "OBJLoader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Shader.h"
#include "glm/gtc/type_ptr.hpp"

int Renderer::drawCalls = 0;

void Renderer:: RenderObj(const std::string &name, Shader &shader, const glm::vec3 &pos, float rot,
                         const glm::vec3 &scale) {

    CustomMesh* mesh = OBJLoader::GetMesh(name);

    if (!mesh) {
        std::cout << "RenderObj: couldn't find mesh name: " << name << std::endl;
        return;
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
    model = glm::scale(model, scale);

    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_model"), 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
}

