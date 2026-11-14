//
// Created by teo95 on 2026-11-14.
//

#ifndef RIDICULOENGINE_TEXTURELOADER_H
#define RIDICULOENGINE_TEXTURELOADER_H
#include <string>
#include <vector>

#include "Mesh.h"
#include "assimp/material.h"


class TextureLoader {
public:
    static std::vector<Texture> loadMaterialTextures(const std::string& directory, aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture> &loaded_textures);
};


#endif //RIDICULOENGINE_TEXTURELOADER_H
