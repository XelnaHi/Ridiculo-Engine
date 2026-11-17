//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_CONTROLLER_H
#define RIDICULOENGINE_CONTROLLER_H
#include "../PlayerInput/PlayerInput.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct Bindings {
    int Forward_W = -1;
    int Backward_S = -1;

    int rotatePitchRight = -1;
    int rotatePitchLeft = -1;
};

class Player;
class Camera;
/*
 * 'Controller' is my name take on an "Input System".
 */
class Controller {
public:
   explicit Controller(GLFWwindow *window, Bindings bindings);

    PlayerInput PollInput();


    Player* player = nullptr;

private:
    GLFWwindow *window;
    Bindings bindings;

    float getAxis(int positive, int negative);
    float getRotation(int positive, int negative);
    bool getKeyDown(int key);
    void createPlayer();

};


#endif //RIDICULOENGINE_CONTROLLER_H
