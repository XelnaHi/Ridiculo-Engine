//
// Created by teo95 on 2026-11-19.
//

#include "SceneParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <../Levels/LevelManager.h>


void SceneParser::ParseSceneFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to load OBJ file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        Scene scene;
        std::string directory = "Levels/";
        std::string levelFileName;

        std::stringstream ss(line);

        ss >> scene.sceneName >> levelFileName;

        scene.levelFilepath = directory + levelFileName + ".txt";

        LevelManager::scenes.emplace_back(scene);
    }
}

void SceneParser::ParseLevel(const std::string &levelFilepath) {
    std::ifstream file(levelFilepath);
    if (!file.is_open()) {
        std::cout << "Failed to load level file: " << levelFilepath << std::endl;
        return;
    }
    std::string line;
    Level level;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;

        ss >> prefix;

        if (prefix == "#M") {
            GameObject go;
            ss >> go.modelName >> go.position.x >> go.position.y >> go.position.z;
            level.gameObjects.emplace_back(go);
        }

        if (prefix == "#P_Start") {
            ss >> level.playerPosition1.x >> level.playerPosition1.y >> level.playerPosition1.z;
        }

    }
        level.levelName = "testTeo";
        LevelManager::levels.emplace_back(level);
}
