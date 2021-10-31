#include "shaderApi.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

string readFromFile(const string &filePath) {
    std::stringstream buffer;
    if (ifstream myFile(filePath); myFile.is_open()) {
        buffer << myFile.rdbuf();
    }
    return buffer.str();
}

unsigned int ShaderUtils::CompileShader(unsigned int type, const string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        cout << "Failed to compile " + shaderType + " shader!" << endl;
        cout << message << endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int ShaderUtils::CreateShader(const string &vertexShader, const string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

unsigned int
ShaderUtils::CreateShaderFromFiles(const string &vertexShaderFilePath, const string &fragmentShaderFilePath) {
    string vertexShaderSource = readFromFile(vertexShaderFilePath);
    string fragmentShaderSource = readFromFile(fragmentShaderFilePath);

    return CreateShader(vertexShaderSource, fragmentShaderSource);
}

ShaderProgram
ShaderProgram::createShaderProgramFromFiles(const std::string &vertexShaderFilePath,
                                            const std::string &fragmentShaderFilePath) {
    unsigned int shaderProgramId = ShaderUtils::CreateShaderFromFiles(vertexShaderFilePath, fragmentShaderFilePath);
    return ShaderProgram(shaderProgramId);
}

ShaderProgram
ShaderProgram::createShaderProgramFromStrings(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int shaderProgramId = ShaderUtils::CreateShader(vertexShader, fragmentShader);
    return ShaderProgram(shaderProgramId);
}

ShaderProgram::ShaderProgram(const unsigned int &inputId) : id(inputId) {}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void ShaderProgram::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::use() {
    glUseProgram(id);
}

unsigned int ShaderProgram::getShaderProgramId() {
    return id;
}
