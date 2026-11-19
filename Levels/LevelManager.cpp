//
// Created by teo95 on 2026-11-19.
//

#include "LevelManager.h"

#include <iostream>
#include <map>

#include "../Controller/Controller.h"
#include "../GameState/GameState.h"
#include "../Renderer/Renderer.h"
#include "../SceneParser/SceneParser.h"
#include "../Player/Player.h"
#include "../Camera/Camera.h"

std::vector<Scene> LevelManager::scenes;
std::map<unsigned int, Level> LevelManager::levels;
Level *LevelManager::activeLevel;


void LevelManager::InitLevel(unsigned int id) {
    const std::string levelPath = scenes[id].levelFilepath;
    Level *level = SceneParser::ParseLevel(id, levelPath);

    if (!level) {
        std::cout << "Level: " << levelPath << " could not be parsed" << std::endl;
        return;
    }

    for (int i = 0; i < level->gameObjects.size(); ++i) {
        Renderer::LoadObj(level->gameObjects[i].modelPath);
    }

    Renderer::LoadObj(level->playerModel1);
    Renderer::LoadObj(level->playerModel2);

    Shader playerShader = Renderer::shaderMap.at("SuzanneShader");

    Renderer::RenderObj(level->playerModel1, playerShader, level->playerPosition1, 0);
    GameState::controllers[0].player->camera->Position = level->playerPosition1;

    Renderer::RenderObj(level->playerModel2, playerShader, level->playerPosition2, 0);
    GameState::controllers[1].player->camera->Position = level->playerPosition2;


    activeLevel = level;
}
