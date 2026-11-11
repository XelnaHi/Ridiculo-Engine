//
// Created by Teo on 2026-11-10.
//

#ifndef RIDICULOENGINE_MODEL_H
#define RIDICULOENGINE_MODEL_H
#include <string>
#include <vector>

#include "Mesh.h"
#include "assimp/scene.h"


class Mesh;
class Shader;

class Model {
public:
    Model(const std::string &path);

    void draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);

    // 'ai' stands for 'Assimp node'
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //RIDICULOENGINE_MODEL_H
