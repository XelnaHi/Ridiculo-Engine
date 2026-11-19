//
// Created by teo95 on 2026-11-17.
//

#include "Controller.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../Levels/LevelManager.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


Controller::Controller(GLFWwindow *window, Bindings inBindings) : window(window), bindings(inBindings) {
    if (!areUtilBindsSet) {
        glfwSetKeyCallback(window, key_callback);
        areUtilBindsSet = true;
    }

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

    input.rotateYaw = getRotation(bindings.rotateYawRight, bindings.rotateYawLeft);
    input.rotatePitch = getRotation(bindings.rotatePitchUp, bindings.rotatePitchDown);

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

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // TODO: refactor how perspective/orthographic is handled
    // if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    //     Renderer::isPerspective = !Renderer::isPerspective;
    //     Renderer::isProjectionDirty = true;
    // }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        LevelManager::InitLevel(0);
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        LevelManager::InitLevel(1);
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        LevelManager::InitLevel(2);
    }
}