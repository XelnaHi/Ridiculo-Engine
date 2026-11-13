//
// Created by Teo on 2026-11-11.
//

#include "OBJLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "CustomMesh.h"

std::unordered_map<std::string, CustomMesh*> OBJLoader::cachedMeshes;

void OBJLoader::LoadMesh(const std::string &filepath) {

    // Load the cached mesh if it has already been loaded and stored. Storing happens at the end of this function.
    auto cachedMesh = cachedMeshes.find(filepath);
    if (cachedMesh != cachedMeshes.end() ) {
        std::cout << "Loaded cached obj model: " << cachedMesh->second->objModelName << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_uvs;

    std::vector<CustomVertex> vertices;
    std::vector<unsigned int> indices;

    std::string objModelName;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Failed to load OBJ file: " << filepath << std::endl;
        return;
    }

    std::string line;

    // reads the opened file one line at a time until the file ends, at which point std::getline(..) will return false.
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;

        // The '>>' extraction operator skips all potential leading whitespaces until it encounters the first non-whitespace character, and starts reading from there
        // >> reads from the 'ss' stream (skipping all leading white spaces) until it encounters the next whitespace, at which point it writes the first token value into our prefix variable. The "stopping" whitespace is not consumed, and will thus be skipped by the next extraction operation.

        /*
         * Example;
         * "s someWord anotherWord"
         *
         * The first extraction would extract the token "s" and assign it to our prefix variable. If we extract again, the token "someWord" would be read, with the leading white space skipped.
         */
        ss >> prefix;

        if (prefix == "o") {
            ss >> objModelName;
        }

        // v = position
        if (prefix == "v") {
            glm::vec3 pos;

            /* Say we're reading a line from the obj file that reads like this: "v 0.435 0.1642 0.7531", the below extraction operations work something like the following;
             *
             * 1. Extracts "0.435" and assigns it to the vec3 pos.x component. Remember that since we already extracted the prefix "v" it has been removed from the buffer
             * and our cursor has been placed at the point of the stopping whitespace.
             *
             *2. Extracts "0.1642"2 and assigns it to the vec3 pos.y component.
             *
             *3. Extracts "0.7531" and assigns it to the vec3 pos.z component.
            */
            ss >> pos.x >> pos.y >> pos.z;
            // Result: We now have a prefix specifying the type of vertex attribute we are currently reading related data to,
            // stored in our "prefix" variable, as well as component data building our vertex attribute, stored in our "pos" variable.

            temp_positions.push_back(pos);
        }
        // vt = texture coordinates
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        // vn = normals
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // f = faces
        // A face is a list of vertex position/uv/normal indices where each "face token" describes the position, uv and normal values for each vertex that together create either a: triangle, quad or n-gon.
        // Faces contains indices pointing to related vertex positions, uvs and normals. For example, "f 11/20/1" would be pointing to a vertex' position index (11), the UV index (20) and the normal index (1).
        // REMEMBER: obj files use 1-based indexing as opposed to the otherwise common programming practice to use 0-based indexing.
        else if (prefix == "f") {
            std::vector<std::string> faceTokens;
            std::string token;
            while (ss >> token) {
                faceTokens.push_back(token);
            }

            // We don't want to handle faces that only describe lines as opposed to triangles
            if (faceTokens.size() >= 3) {
                // This pattern is called 'Fan Triangulation'
                // The formula for how many triangles that will be constructed is: AmountOfFaceTokens - 1 - 1. Given a shape with 6 face tokens, that gives us 4 triangles.
                for (size_t i = 1; i < faceTokens.size() - 1; i++) {
                    // This pattern allows us to re-construct triangles within a shape, using the first vertex index as the first vertex position for each reconstructed triangle, and only updating the second and third index positions
                    int triangleIndices[3] = {0, static_cast<int>(i), static_cast<int>(i + 1)};

                    for (int k = 0; k < 3; ++k) {
                        std::string s = faceTokens[triangleIndices[k]];

                        // Say we have the face token 11/20/1
                        // We first retrieve the index of the '/' character (idx 2) from the string value
                        size_t firstSlash = s.find('/');
                        // Then the index of the second '/' (idx 5)
                        size_t secondSlash = s.find('/', firstSlash + 1);

                        // Now we convert that string value to a numerical value, starting from the first string index and reading up until the point of the index of the first '/'.

                        // Then we subtract 1 from it. The result looks something like this;

                        // substr returns the token "11".

                        // --> stoi converts it to a numerical value: 11.

                        // --> obj files use 1-based indexing, but we don't in our code, so we subtract 1 from 11 in order to map the 1-based indexing to our 0-based indexing.
                        unsigned int pIdx = std::stoi(s.substr(0, firstSlash)) - 1;
                        int uvIdx = -1;   // -1 means "this vertex has no UV"
                        int nIdx = -1;    // -1 means "this vertex has no normal"

                        // npos equals the maximum value representable by the type size_type, usually indicating 'end of string' or as an error indication used by functions that return a string index.
                        if (firstSlash != std::string::npos) {
                            // I guess here we make sure that the face token does't look like this "11//1", which would mean that we're missing a uv?
                            if (secondSlash != firstSlash + 1) {
                                // Example:
                                // Get (secondSlash - firstSlash - 1) characters, starting from index (firstSlash + 1)
                                // --> idx = (secondSlash + 1) = (2 + 1) = 3
                                // --> amount of characters = (secondSlash - firstSlash - 1) = (5 - 2 - 1) = 2
                                // Result == get 2 characters starting from index 3 --> retrieves the value (20, given our above face token example)
                                // stoi converts the string "20" to a numerical value.
                                // --> subtract 1 from the numerical value. Once again, this is 1-based indexing mapped to 0-based indexing.
                                // uvIdx = 19

                                size_t uvLen = (secondSlash == std::string::npos) ? std::string::npos
                                                                                  : secondSlash - firstSlash - 1;
                                uvIdx = std::stoi(s.substr(firstSlash + 1, uvLen)) - 1;
                            }
                            // Similarly to the above if check, I guess here we are checking that we are not missing a normal?
                            if (secondSlash != std::string::npos) {
                                nIdx = std::stoi(s.substr(secondSlash + 1)) - 1;
                            }
                        }

                        CustomVertex vertex;

                        vertex.position = temp_positions[pIdx];
                        vertex.normal = (nIdx >= 0 && nIdx < (int)temp_normals.size()) ? temp_normals[nIdx] : glm::vec3(0.0f, 1.0f, 0.0f);
                        vertex.uv     = (uvIdx >= 0 && uvIdx < (int)temp_uvs.size())   ? temp_uvs[uvIdx]   : glm::vec2(0.0f);

                        int existingIndex = -1;
                        for (size_t j = 0; j < vertices.size(); ++j) {
                            if (vertices[j].position == vertex.position &&
                                vertices[j].normal == vertex.normal &&
                                vertices[j].uv == vertex.uv) {
                                existingIndex = (int) j;
                                break;
                            }
                        }
                        if (existingIndex != -1) {
                            indices.push_back((unsigned int) existingIndex);
                        } else {
                            indices.push_back((unsigned int) vertices.size());
                            vertices.push_back(vertex);
                        }
                    }
                }
            }
        }
    }

    CustomMesh* mesh = new CustomMesh();
    mesh->SetupMesh(vertices, indices);
    cachedMeshes[filepath] = mesh;
    mesh->objModelName = objModelName;

    std::cout << "Loaded OBJ model: " << objModelName << std::endl;
}

CustomMesh * OBJLoader::GetMesh(const std::string &key) {
    auto cachedMesh = cachedMeshes.find(key);

    return cachedMesh != cachedMeshes.end() ? cachedMesh->second : nullptr;
}
