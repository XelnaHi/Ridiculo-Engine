//
// Created by Teo on 2026-11-10.
//

#include "Model.h"

#include <iostream>
#include <ostream>

#include "../Shader.h"
#include "./Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../stb/stb_image.h"
#include "glm/gtc/matrix_transform.hpp"

static int modelNr;

std::unordered_map<std::string, Model *> Model::cachedModels;

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);


Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::draw(Shader &shader, const glm::vec3 &pos, float rot, const glm::vec3 &scale) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, pos, rot, scale);
    }
}

void Model::drawTest(const std::string &name, Shader &shader, const glm::vec3 &pos, float rot, const glm::vec3 &scale) {

    if (!source) {
        return;
    }

    for (auto &mesh : source->meshes) {
        mesh.draw(shader, pos, rot, scale);
    }
}


void Model::loadModel(const std::string &path) {
    auto cachedModel = cachedModels.find(path);
    if (cachedModel != cachedModels.end()) {

        source = cachedModel->second;
        std::cout << "Loaded cached obj model: " << source->objModelName << std::endl;
        return;
    }
    source = this;


    Assimp::Importer importer;
    // The below call creates and assigns a scene object that stores all data related to meshes, textures, UV's, and materials.
    // The below call also abstracts away the necessity for per-file-format management we would otherwise have to do when importing a model.
    // aiProcess_Triangulate is a post-processing argument (same as aiProcess_FlipUVs) that force Assimp to recalculate all primitive shapes as triangles, if the model does not entirely consist of triangles (I assume this is a foreshadowing of n-gons).
    // aiProcess_FlipUVs flips the tex coords on the y-axis if necessary. Most images in OpenGL are reversed around the y-axis, making this a necessity.

    // Assimp provides a few other postprocessing options but I will refer myself to the LearnOpenGL docs should I need them: https://learnopengl.com/Model-Loading/Model (Note: The link learnopengl themselves reference is broken, but they still provide an overview of the postproocessors themselves.)
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

    cachedModels[path] = this;
    objModelName = scene->mRootNode->mName.C_Str();

    std::cout << "Loaded assimp model: " << scene->mRootNode->mName.C_Str() << std::endl;
}

/*
 * A recursive function that processes each node within the Assimp scene object
 */
void Model::processNode(aiNode *node, const aiScene *scene) {
    /*
     * A rough overview of Assimp's scene data structure
     * Scene - contains the root node, a list of meshes, and a list of indices to materials
     * Root node - contains a list of child nodes and a list of meshes indices where each index points to a specific mesh
     * Child node - contains a list of child nodes and a list of meshes indices where each index points to a specific mesh
     * Mesh - contains a list of vertices, normals, tex coords, face indices, and material indices
     * Face - contains a list of indices
     *
     * The stated reason for Assimp's data structure consisting of children that eventually point back to the scene which contains the the actual mesh and material objects as opposed to just looping over the scenes list of meshes and materials in order to process them is that this kind of data structure creates a parent-child relationship which makes it a lot easier to transform all child meshes based on the parent. The example learnopengl gives is that of a 'car mesh' (model, I assume), consisting of meshes related to the car doors, tires, windows, etc, and how they can all easily be transformed all together given that a parent->child structure exists.
     */

    // process all the node's meshes, if any
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // Recursively process each potential child mesh
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // This will store the combination of vertex positions, normals and tex coords
        Vertex vertex;

        // A vector used to temporarily store the component values of the vertex positions and normals
        glm::vec3 vector;

        // Process vertices
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Process vertex normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Process vertex tex coords
        if (mesh->mTextureCoords[0]) {
            glm::vec2 texVec;
            texVec.x = mesh->mTextureCoords[0][i].x;
            texVec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texVec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

/*
 * A helper function that iterates over all the texture locations of the given texture type, retrieves the texture's file location and then loads and generates the texture and stores the information in a Vertex struct.
 *
 * The material object contains various texture types, such as specular and/or diffuse textures. Assimp prefixes these textures with 'aiTextureType_'.
 */
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
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

Model *Model::GetModel(const std::string &key) {
    auto cachedModel = cachedModels.find(key);

    return cachedModel != cachedModels.end() ? cachedModel->second : nullptr;
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
