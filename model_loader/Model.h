//
// Created by Teo on 2026-11-10.
//

#ifndef RIDICULOENGINE_MODEL_H
#define RIDICULOENGINE_MODEL_H
#include <string>
#include <vector>

#include "assimp/scene.h"


struct Texture;
class Mesh;
class Shader;

class Model {
public:
    Model(char *path);

    void draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);

    // 'ai' stands for 'Assimp node'
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //RIDICULOENGINE_MODEL_H
