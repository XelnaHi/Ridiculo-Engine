//
// Created by teo95 on 2026-11-18.
//


#include "GameState.h"

#include <iostream>

#include "../Controller/Controller.h"
#include "../Renderer/Renderer.h"

std::vector<Controller> GameState::controllers;
std::vector<PlayerInput> GameState::playerInputs;


void GameState::CreateController(GLFWwindow *window, const Bindings &bindings) {
    Controller controller(window, bindings);
    controllers.emplace_back(controller);

    playerInputs.emplace_back();
}

void GameState::RemovePlayer() {
    if (GameState::controllers.empty()) {
        std::cout << "There are no controllers in left in the list" << std::endl;
    } else {
        GameState::controllers.pop_back();
    }

    if (GameState::playerInputs.empty()) {
        std::cout << "There are no playerInputs left in the list" << std::endl;
    } else {
        GameState::playerInputs.pop_back();
    }

    // Not an elegant solution, but will do for now. The problem this solves is that if we remove one player but dont
    // mark the projection as dirty, no recalculation of the projection matrix will be made, resulting in a
    // stretched view, if going from 2 players to 1.
    Renderer::isProjectionDirty = true;
}


void GameState::AddPlayer(GLFWwindow *window) {

    // Default binding for a second player. This assumes (and we gatekeep it) that dont allow more than 2 players
    // to exist.
    if (controllers.size() == 2) {
        std::cout << "Two players already exist. Max amount of allowed players is two." << std::endl;
        return;
    }

    Bindings defaultBinding = {
        .Forward_W = GLFW_KEY_UP,
        .Backward_S = GLFW_KEY_DOWN,
        .rotateYawRight = GLFW_KEY_RIGHT,
        .rotateYawLeft = GLFW_KEY_LEFT,
        .rotatePitchUp = GLFW_KEY_PAGE_DOWN,
        .rotatePitchDown = GLFW_KEY_PAGE_UP
    };

    CreateController(window, defaultBinding);
    Renderer::isProjectionDirty = true;
}
