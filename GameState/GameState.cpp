//
// Created by teo95 on 2026-11-18.
//


#include "GameState.h"

#include <iostream>

#include "../Controller/Controller.h"

std::vector<Controller> GameState::controllers;
std::vector<PlayerInput> GameState::playerInputs;


void GameState::CreateController(GLFWwindow *window, const Bindings& bindings) {
    Controller controller(window, bindings);
    controllers.emplace_back(controller);

    playerInputs.emplace_back();
}
