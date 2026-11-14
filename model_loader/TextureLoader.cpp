//
// Created by teo95 on 2026-11-14.
//

#include "TextureLoader.h"

#include <iostream>

#include "glad/glad.h"
#include "../stb/stb_image.h"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

/*
 * A helper function that iterates over all the texture locations of the given texture type, retrieves the texture's file location and then loads and generates the texture and stores the information in a Vertex struct.
 *
 * The material object contains various texture types, such as specular and/or diffuse textures. Assimp prefixes these textures with 'aiTextureType_'.
 */
std::vector<Texture> TextureLoader::loadMaterialTextures(const std::string &directory, aiMaterial *mat,
                                                         aiTextureType type, std::string typeName, std::vector<Texture> &textures_loaded) {
    std::vector<Texture> textures;

    // GetTextureCount(type) expects a texture type and returns the amount of defined textures of said type.
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;

        // Retrieves the file location of the currently iterated over texture. Stores the result in 'str'.
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            // strcmp(..) compares two null-terminated strings and returns 0 if they are equal, or a negative value if the first string precedes the second, and a positive value if the first string follows the second.
            // The comparison is case-sensitive and is basd on the ASCII values of the characters. Returns undefined behavior if one at least one of the strings is not a pointer to a null-terminated string.
            if (std::strcmp(textures_loaded[j].Path.data(), str.C_Str()) == 0) {
                // Strings were equal, meaning the texture has already been loaded. Store the loaded texture string.
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;

            // Helper function that loads the texture and returns the texture ID.
            texture.ID = TextureFromFile(str.C_Str(), directory);

            texture.Type = typeName;
            texture.Path = str.C_Str();

            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}


/*
 * This helper function that loads a texture file from a folder assumes that the texture file is located relative to where the model is.
 * Below is an example of how the texture reader expects the .obj, .mtl and textures are expected to be placed;
 * - models/backpack/backpack.obj
 * - models/backpack/backpack.mtl
 * - models/backpack/normal.jpg
 * - models/backpack/normal.diffuse
 * - models/backpack/normal.roughness
 * - etc
 */
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}