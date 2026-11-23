//
// Created by teo95 on 2026-11-19.
//

#include "LevelManager.h"

#include <iostream>
#include <map>

#include "../GameState/GameState.h"
#include "../Renderer/Renderer.h"
#include "../SceneParser/SceneParser.h"


std::vector<Scene> LevelManager::scenes;
std::map<unsigned int, Level> LevelManager::levels;
Level *LevelManager::activeLevel;

void LevelManager::SwitchLevel(unsigned int id) {
        const Level* level = Activate(id);
        if (!level) return;
        Renderer::PrepareLevel(*level);
        GameState::PlacePlayersAtStart(*level);
}


Level *LevelManager::Activate(unsigned int id) {
    Level *level = SceneParser::ParseLevel(id, scenes[id].levelFilepath);
    if (!level) {
        std::cout << "Level " << id << " could not be parsed\n";
        return nullptr;
    }
    activeLevel = level;
    return level;
}
