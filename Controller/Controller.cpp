//
// Created by teo95 on 2026-11-17.
//

#include "Controller.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../GameState/GameState.h"
#include "../Levels/LevelManager.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


Controller::Controller(GLFWwindow *window, Bindings inBindings) : window(window), bindings(inBindings) {
    // if (!areUtilBindsSet) {
    //     glfwSetKeyCallback(window, key_callback);
    //     areUtilBindsSet = true;
    // }
    //
    // // Hides the cursor and captures it.
    // // Capturing the cursor means that once the application has gained focus,
    // // the mouse stays within the bounds of the window.
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //
    // double x, y;
    // // Returns the cursor position in screen coordinates relative to the upper left content area
    // // of the specified window.
    //
    // // Going to skip this for now, as implementing rotation for two players on one keyboard seems overscope
    // // for now.
    // // glfwGetCursorPos(window, &x, &y);

    createPlayer();
}

PlayerInput Controller::PollInput(const InputSystem& inputSystem) {
    PlayerInput input;


    input.moveZ = getAxis(inputSystem, bindings.Forward_W, bindings.Backward_S);

    input.rotateYaw = getAxis(inputSystem, bindings.rotateYawRight, bindings.rotateYawLeft);
    input.rotatePitch = getAxis(inputSystem, bindings.rotatePitchUp, bindings.rotatePitchDown);

    return input;
}

void Controller::createPlayer() {
    auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto newPlayer = new Player(camera);
    player = newPlayer;
}


float Controller::getAxis(const InputSystem& input, const int positive, const int negative) {
    return (input.IsKeyDown(positive) ? 1.0f : 0.0f) - (input.IsKeyDown(negative) ? 1.0f : 0.0f);
}

