//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_OBJLOADER_H
#define RIDICULOENGINE_OBJLOADER_H
#include <string>


class CustomMesh;

class OBJLoader {
    public:
    static CustomMesh* LoadMesh(const std::string& filepath);
};


#endif //RIDICULOENGINE_OBJLOADER_H
