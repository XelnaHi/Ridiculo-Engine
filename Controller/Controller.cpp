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

Controller::Controller(GLFWwindow *window, Bindings inBindings) : window(window), bindings(inBindings) {

}

PlayerInput Controller::PollInput(const InputSystem& inputSystem) {
    PlayerInput input;

    input.moveZ = getAxis(inputSystem, bindings.Forward_W, bindings.Backward_S);

    input.rotateYaw = getAxis(inputSystem, bindings.rotateYawRight, bindings.rotateYawLeft);
    input.rotatePitch = getAxis(inputSystem, bindings.rotatePitchUp, bindings.rotatePitchDown);

    return input;
}


float Controller::getAxis(const InputSystem& input, const int positive, const int negative) {
    return (input.IsKeyDown(positive) ? 1.0f : 0.0f) - (input.IsKeyDown(negative) ? 1.0f : 0.0f);
}

