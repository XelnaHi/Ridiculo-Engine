//
// Created by Teo on 2026-11-23.
//
#include "InputSystem.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


InputSystem::InputSystem(GLFWwindow *window) : window(window) {
        glfwSetKeyCallback(window, key_callback);
}



bool InputSystem::IsKeyDown(const int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
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
        // LevelManager::InitLevel(0);
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        // LevelManager::InitLevel(1);
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        // LevelManager::InitLevel(2);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        // GameState::RemovePlayer();
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        // GameState::AddPlayer(window);
    }
}