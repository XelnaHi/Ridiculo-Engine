//
// Created by teo95 on 2026-11-19.
//

#ifndef RIDICULOENGINE_LEVELMANAGER_H
#define RIDICULOENGINE_LEVELMANAGER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct GameObject;

struct Scene {
    std::string sceneName;
    std::string levelFilepath;
};

struct Level {
    std::string levelName;
    glm::vec3 playerPosition1;
    glm::vec3 playerPosition2;
    std::vector<GameObject> gameObjects;
};

struct GameObject {
    std::string modelName;
    glm::vec3 position;
};

class LevelManager {
public:
    static std::vector<Scene> scenes;
    static std::vector<Level> levels;

};


#endif //RIDICULOENGINE_LEVELMANAGER_H
