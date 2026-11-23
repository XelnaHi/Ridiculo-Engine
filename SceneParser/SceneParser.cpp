//
// Created by teo95 on 2026-11-19.
//

#include "SceneParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <../Levels/LevelManager.h>
#include <../Player/Player.h>


void SceneParser::ParseSceneFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to load OBJ file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::string directory = "Levels/";

    while (std::getline(file, line)) {
        Scene scene;
        std::string levelFileName;

        std::stringstream ss(line);

        ss >> scene.sceneName >> levelFileName;

        scene.levelFilepath = directory + levelFileName + ".txt";

        LevelManager::scenes.emplace_back(scene);

        std::cout << "Scene " << scene.sceneName << " loaded" << std::endl;
    }
}

Level *SceneParser::ParseLevel(unsigned int id, const std::string &levelFilepath) {
    if (LevelManager::levels.contains(id)) {
        std::cout << "Level with ID " << id << " already exists" << std::endl;
        return &LevelManager::levels[id];
    }

    std::ifstream file(levelFilepath);
    if (!file.is_open()) {
        std::cout << "Failed to load level file: " << levelFilepath << std::endl;
        return nullptr;
    }
    std::string line;

    Level level;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;

        ss >> prefix;

        if (prefix == "#ID") {
            ss >> level.ID;
        }

        if (prefix == "#M") {
            GameObject go;
            ss >> go.modelPath >> go.position.x >> go.position.y >> go.position.z >> go.scale.x >> go.scale.y >> go.
                    scale.z;
            level.gameObjects.emplace_back(go);
        }

        if (prefix == "#PlayerInfo") {
            PlayerInfo playerInfo;

            ss >> playerInfo.playerModel >> playerInfo.startPosition.x >> playerInfo.startPosition.y >>
                    playerInfo.startPosition.z;
            level.playerInfoList.emplace_back(playerInfo);
        }

    }

    auto [it, inserted] = LevelManager::levels.emplace(level.ID, level);
    if (!inserted) {
        std::cout << "Failed to load level file: " << levelFilepath << std::endl;
        return nullptr;
    }

    return &it->second;
}
