//
// Created by Teo on 2026-11-11.
//

#ifndef RIDICULOENGINE_OBJLOADER_H
#define RIDICULOENGINE_OBJLOADER_H
#include <string>
#include <unordered_map>


class CustomMesh;

class OBJLoader {
    public:
    static CustomMesh* LoadMesh(const std::string& filepath);

private:
    static std::unordered_map<std::string, CustomMesh*> cachedMeshes;
};


#endif //RIDICULOENGINE_OBJLOADER_H
