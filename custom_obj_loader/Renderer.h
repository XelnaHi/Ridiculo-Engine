//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_RENDERER_H
#define RIDICULOENGINE_RENDERER_H
#include <string>

#include <glm/glm.hpp>

class Model;
class Shader;


class Renderer {
public:
    static void LoadObj(const std::string& path);
    static void RenderObj(const std::string& name, Shader& shader, const glm::vec3& pos, float rot, const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f) );
    static inline bool useCustomLoader = true;
    static int drawCalls;
};


#endif //RIDICULOENGINE_RENDERER_H
