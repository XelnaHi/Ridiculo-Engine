//
// Created by teo95 on 2026-11-14.
//

#ifndef RIDICULOENGINE_TEXTURELOADER_H
#define RIDICULOENGINE_TEXTURELOADER_H
#include <string>
#include <unordered_map>
#include <vector>

#include "Mesh.h"
#include "assimp/material.h"


class TextureLoader {
public:
    static std::vector<Texture> loadMaterialTextures(const std::string& directory, aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture> &loaded_textures);

    static unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

private:
    static std::unordered_map<std::string, unsigned int> cachedTextures;
};


#endif //RIDICULOENGINE_TEXTURELOADER_H
