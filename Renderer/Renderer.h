//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_RENDERER_H
#define RIDICULOENGINE_RENDERER_H
#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLFW/glfw3.h"
#include "../Shader.h"

class Controller;
class Model;


class Renderer {
public:

    static inline bool useCustomLoader = true;
    static int drawCalls;

    static void Render(GLint viewportWidth);

    static void LoadObj(const std::string &path);

    static void RenderObj(const std::string &name, Shader &shader, const glm::vec3 &pos, float rot,
                          const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));


};


#endif //RIDICULOENGINE_RENDERER_H
