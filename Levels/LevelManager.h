//
// Created by teo95 on 2026-11-19.
//

#ifndef RIDICULOENGINE_LEVELMANAGER_H
#define RIDICULOENGINE_LEVELMANAGER_H
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Player;
struct GameObject;

struct Scene {
    std::string sceneName;
    std::string levelFilepath;
};

struct Level {
    unsigned int ID;
    std::string levelName;
    std::vector<GameObject> gameObjects;

    glm::vec3 playerPosition1;
    std::string playerModel1;

    glm::vec3 playerPosition2;
    std::string playerModel2;
};

struct GameObject {
    std::string modelPath;
    glm::vec3 position;
    glm::vec3 scale;
};

class LevelManager {
public:
    static void InitLevel(unsigned int id);

    static std::vector<Scene> scenes;
    static std::map<unsigned int, Level> levels;
    static Level* activeLevel;
};


#endif //RIDICULOENGINE_LEVELMANAGER_H
