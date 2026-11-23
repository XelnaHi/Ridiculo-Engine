//
// Created by teo95 on 2026-11-18.
//


#include "GameState.h"

#include <iostream>

#include "../Controller/Controller.h"
#include "../GameRules/GameRules.h"
#include "../Player/Player.h"
#include "../Renderer/Renderer.h"
#include "../Levels/LevelManager.h"

std::vector<PlayerSlot> GameState::players;

void GameState::PlacePlayersAtStart(const Level &level) {
    const size_t n = std::min(players.size(), level.playerInfoList.size());
    for (size_t i = 0; i < n; ++i)
        players[i].player->TeleportTo(level.playerInfoList[i].startPosition);
}


void GameState::RemovePlayer() {
    if (players.size() < 2) {
        std::cout << "Only one player left. Can't delete the last player while game is still active" << std::endl;
    } else {
        players.pop_back();
    }

    // Not an elegant solution, but will do for now. The problem this solves is that if we remove one player but dont
    // mark the projection as dirty, no recalculation of the projection matrix will be made, resulting in a
    // stretched view, if going from 2 players to 1.
}


void GameState::AddPlayer
(GLFWwindow *window, const Bindings &bindings) {
    // Default binding for a second player. This assumes (and we gatekeep it) that dont allow more than 2 players
    // to exist.
    if (players.size() == GameRules::MAX_PLAYERS) {
        std::cout << "Two players already exist. Max amount of allowed players is two." << std::endl;
        return;
    }

    Player player;
    Controller controller(window, bindings);
    players.emplace_back(controller, player);
}

void GameState::AddPlayer(GLFWwindow *window) {
    if (players.size() == GameRules::MAX_PLAYERS) {
        std::cout << "There are no controllers in left in the list" << std::endl;
        return;
    }
    const Bindings defaultBindings = {
        GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_PAGE_DOWN, GLFW_KEY_PAGE_UP
    };

    Player player;
    Controller controller(window, defaultBindings);
    players.emplace_back(controller, player);
}
