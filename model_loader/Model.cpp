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

#include "TextureLoader.h"
#include "../stb/stb_image.h"
#include "glm/gtc/matrix_transform.hpp"


std::unordered_map<std::string, Model *> Model::cachedModels;

Model::Model(const std::string &path) {
    loadModel(path);
}

Model * Model::Load(const std::string &path) {
    auto it = cachedModels.find(path);
    if (it != cachedModels.end()) {
        std::cout << "Loaded cached obj model: " << it->second->objModelName << std::endl;
        return it->second;
    }
    auto model = new Model(path);
    cachedModels[path] = model;
    return model;
}

void Model::draw(Shader &shader, const glm::vec3 &pos, float rot, const glm::vec3 &scale, unsigned int overrideDiffuseID) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, pos, rot, scale, overrideDiffuseID);
    }
}

void Model::loadModel(const std::string &path) {
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

        std::vector<Texture> diffuseMaps = TextureLoader::loadMaterialTextures(directory, material, aiTextureType_DIFFUSE, "texture_diffuse", textures_loaded);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = TextureLoader::loadMaterialTextures(directory, material, aiTextureType_SPECULAR, "texture_specular", textures_loaded);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps = TextureLoader::loadMaterialTextures(directory, material, aiTextureType_HEIGHT, "texture_normal", textures_loaded);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    return Mesh(vertices, indices, textures);
}


Model *Model::GetModel(const std::string &key) {
    auto cachedModel = cachedModels.find(key);

    return cachedModel != cachedModels.end() ? cachedModel->second : nullptr;
}
