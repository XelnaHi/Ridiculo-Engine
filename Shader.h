//
// Created by teodor.fredriksson on 2026-10-30.
//

#ifndef RIDICULOENGINE_SHADER_H
#define RIDICULOENGINE_SHADER_H

#include <glad/glad.h>
#include <string>

class Shader
{
public:
    unsigned int ID;

    // Read an build shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use/activate the shader
    void use();

    // Utility
    void setBool(const std::string &name, bool value) const;
    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
};


#endif //RIDICULOENGINE_SHADER_H
