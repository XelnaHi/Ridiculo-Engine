//
// Created by teo95 on 2026-11-17.
//

#include "Controller.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../Player/Player.h"

Controller::Controller(GLFWwindow *window) : window(window) {
    // Hides the cursor and captures it.
    // Capturing the cursor means that once the application has gained focus,
    // the mouse stays within the bounds of the window.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double x, y;
    // Returns the cursor position in screen coordinates relative to the upper left content area
    // of the specified window.

    // Going to skip this for now, as implementing rotation for two players on one keyboard seems overscope
    // for now.
    // glfwGetCursorPos(window, &x, &y);

    createPlayer();
}

PlayerInput Controller::PollInput() {
    PlayerInput input;

    input.moveZ = getAxis(0,0);

    return input;
}

void Controller::createPlayer() {
    auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto newPlayer = new Player(camera);
    player = newPlayer;
}

float Controller::getAxis(int positive, int negative) {
    return getKeyDown() ? 1.0 : 0.0;
}

bool Controller::getKeyDown() {
    int state = glfwGetKey(window, GLFW_KEY_W);
    if (state == GLFW_PRESS) {
        return true;
    }
    return false;
}
