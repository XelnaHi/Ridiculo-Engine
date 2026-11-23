//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_PLAYERINPUT_H
#define RIDICULOENGINE_PLAYERINPUT_H
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct PlayerInput {
    float moveX = 0.0f;
    float moveY = 0.0f;
    float moveZ = 0.0f;

    float rotateYaw = 0.0f;
    float rotatePitch = 0.0f;
};

class InputSystem {
public:
    explicit InputSystem(GLFWwindow *window);

    [[nodiscard]] bool IsKeyDown(int key) const;

private:
    GLFWwindow *window;
};


#endif //RIDICULOENGINE_PLAYERINPUT_H
