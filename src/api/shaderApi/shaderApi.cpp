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
        char *message = (char *) alloca(length * sizeof(char));
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
