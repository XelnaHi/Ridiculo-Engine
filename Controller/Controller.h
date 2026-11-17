//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_CONTROLLER_H
#define RIDICULOENGINE_CONTROLLER_H
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Camera;
/*
 * 'Controller' is my name take on an "Input System".
 */
class Controller {
public:
    Controller(Camera* camera, GLFWwindow *window);

    void  ProcessInput(float deltaTime) const;

    Camera* camera;
    GLFWwindow* window;
};


#endif //RIDICULOENGINE_CONTROLLER_H
