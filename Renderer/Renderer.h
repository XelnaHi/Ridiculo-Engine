//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_RENDERER_H
#define RIDICULOENGINE_RENDERER_H
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLFW/glfw3.h"
#include "../Shader.h"


class RenderSystem;
struct Level;
class Camera;
class Controller;
class Model;


class Renderer {
public:

    static inline bool useCustomLoader = true;
    static int drawCalls;

    static void Render(GLint viewportWidth, int playerIndex, Camera &camera, std::map<std::string, Shader> &shaderMap, RenderSystem& renderSystem);

    static void LoadObj(const std::string &path);

    static void RenderObj(const std::string &name, Shader &shader, const glm::vec3 &pos, float rot,
                          const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));

    static void PrepareLevel(const Level& level);

};


#endif //RIDICULOENGINE_RENDERER_H
