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

struct PlayerInfo {
    glm::vec3 startPosition;
    std::string playerModel;
};

struct Level {
    unsigned int ID;
    std::string levelName;
    std::vector<GameObject> gameObjects;
    std::vector<PlayerInfo> playerInfoList;
};

struct GameObject {
    std::string modelPath;
    glm::vec3 position;
    glm::vec3 scale;
};

class LevelManager {
public:
    static Level* Activate(unsigned int id);

    static std::vector<Scene> scenes;
    static std::map<unsigned int, Level> levels;
    static Level* activeLevel;
    static void SwitchLevel(unsigned int id);
};


#endif //RIDICULOENGINE_LEVELMANAGER_H
