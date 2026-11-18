//
// Created by teo95 on 2026-11-18.
//

#ifndef RIDICULOENGINE_GAMESTATE_H
#define RIDICULOENGINE_GAMESTATE_H
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct Bindings;
struct PlayerInput;
class Controller;

class GameState {
public:
    static void CreateController(GLFWwindow *window, const Bindings &bindings);

    static std::vector<Controller> controllers;
    static std::vector<PlayerInput> playerInputs;
};


#endif //RIDICULOENGINE_GAMESTATE_H
