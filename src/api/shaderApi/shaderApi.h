#pragma once

#include <string>

class ShaderUtils {
public:
    static unsigned int CompileShader(unsigned int type, const std::string &source);

    static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);

    static unsigned int
    CreateShaderFromFiles(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);
};
