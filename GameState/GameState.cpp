//
// Created by teo95 on 2026-11-18.
//


#include "GameState.h"

#include <iostream>

#include "../Controller/Controller.h"
#include "../GameRules/GameRules.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"

std::vector<PlayerSlot> GameState::players;


void GameState::RemovePlayer() {
    if (players.empty()) {
        std::cout << "There are no controllers in left in the list" << std::endl;
    } else {
        players.pop_back();
    }

    if (players.empty()) {
        std::cout << "There are no playerInputs left in the list" << std::endl;
    } else {
        players.pop_back();
    }

    // Not an elegant solution, but will do for now. The problem this solves is that if we remove one player but dont
    // mark the projection as dirty, no recalculation of the projection matrix will be made, resulting in a
    // stretched view, if going from 2 players to 1.
    Camera::isProjectionDirty = true;
}


void GameState::AddPlayer(GLFWwindow *window, const Bindings& bindings) {
    // Default binding for a second player. This assumes (and we gatekeep it) that dont allow more than 2 players
    // to exist.
    if (players.size() == GameRules::MAX_PLAYERS) {
        std::cout << "Two players already exist. Max amount of allowed players is two." << std::endl;
        return;
    }

    Player player;
    Controller controller(window, bindings);
    players.emplace_back(controller, player);

    Camera::isProjectionDirty = true;
}
