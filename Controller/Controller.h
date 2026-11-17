//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_CONTROLLER_H
#define RIDICULOENGINE_CONTROLLER_H
#include "../PlayerInput/PlayerInput.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Player;
class Camera;
/*
 * 'Controller' is my name take on an "Input System".
 */
class Controller {
public:
   explicit Controller(GLFWwindow *window);

    PlayerInput PollInput();


    Player* player = nullptr;

private:
    GLFWwindow *window;

    float getAxis(int positive, int negative);
    bool getKeyDown();
    void createPlayer();

};


#endif //RIDICULOENGINE_CONTROLLER_H
