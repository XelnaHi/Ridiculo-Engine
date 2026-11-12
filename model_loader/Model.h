//
// Created by Teo on 2026-11-10.
//

#ifndef RIDICULOENGINE_MODEL_H
#define RIDICULOENGINE_MODEL_H
#include <string>
#include <unordered_map>
#include <vector>

#include "Mesh.h"
#include "assimp/scene.h"


class Mesh;
class Shader;

class Model {
public:
    Model(const std::string &path);

    void draw(Shader& shader, const glm::vec3& pos, float rot, const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
    void drawTest(const std::string& name, Shader &shader, const glm::vec3& pos, float rot, const glm::vec3& scale);

    std::string objModelName;

    // Used in relation to caching. Cached objects will store a reference to the object it is identical to, and will use the source's meshes to draw, removing the need for us to store anything but a pointer when doing caching.
    Model* source = nullptr;

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(const std::string& path);

    // 'ai' stands for 'Assimp'
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    static Model* GetModel(const std::string& key);


private:
    static std::unordered_map<std::string, Model*> cachedModels;
};


#endif //RIDICULOENGINE_MODEL_H
