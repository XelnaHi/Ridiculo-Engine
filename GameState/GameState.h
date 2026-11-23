//
// Created by teo95 on 2026-11-18.
//

#ifndef RIDICULOENGINE_GAMESTATE_H
#define RIDICULOENGINE_GAMESTATE_H
#include <map>
#include <optional>
#include <vector>

#include "../Controller/Controller.h"
#include "../Player/Player.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct Level;
struct Bindings;
struct PlayerInput;

struct PlayerSlot {
    Controller controller;
    std::optional<Player> player;
};

class GameState {
public:
    static void RemovePlayer();

    static void AddPlayer(GLFWwindow *window, const Bindings &bindings);
    static void AddPlayer(GLFWwindow *window);

    static std::vector<PlayerSlot> players;

    static void PlacePlayersAtStart(const Level& level);
};


#endif //RIDICULOENGINE_GAMESTATE_H
