//
// Created by teo95 on 2026-11-19.
//

#ifndef RIDICULOENGINE_SCENEPARSER_H
#define RIDICULOENGINE_SCENEPARSER_H
#include <string>

class SceneParser {
public:
    static void ParseSceneFile(const std::string& filePath);
    static void ParseLevel(const std::string& levelFilepath);
};


#endif //RIDICULOENGINE_SCENEPARSER_H
