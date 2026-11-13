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
    static void LoadMesh(const std::string& filepath);
    static CustomMesh* GetMesh(const std::string& key);

private:
    static std::unordered_map<std::string, CustomMesh*> cachedMeshes;
};


#endif //RIDICULOENGINE_OBJLOADER_H
