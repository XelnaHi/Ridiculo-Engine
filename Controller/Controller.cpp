//
// Created by teo95 on 2026-11-17.
//

#include "Controller.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../Player/Player.h"

Controller::Controller(GLFWwindow *window, Bindings inBindings) : window(window), bindings(inBindings) {
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

    input.moveZ = getAxis(bindings.Forward_W, bindings.Backward_S);

    input.rotateYaw = getRotation(bindings.rotatePitchRight, bindings.rotatePitchLeft);

    return input;
}

void Controller::createPlayer() {
    auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto newPlayer = new Player(camera);
    player = newPlayer;
}

float Controller::getAxis(int positive, int negative) {
    return (getKeyDown(positive) ? 1.0f : 0.0f) - (getKeyDown(negative) ? 1.0f : 0.0f);
}

float Controller::getRotation(int positive, int negative) {
    return (getKeyDown(positive) ? 1.0f : 0.0f) - (getKeyDown(negative) ? 1.0f : 0.0f);
}

bool Controller::getKeyDown(int key) {
    return key != -1 && glfwGetKey(window, key) == GLFW_PRESS;
}
