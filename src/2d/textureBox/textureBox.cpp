#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include "texturesApi.h"

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

string getTitle();

vector<float> getSquarePositions();

vector<unsigned int> getSquareIndicesPositions();

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 aCoord;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;

void main(){
    gl_Position = aCoord;
    Color = aColor;
    TexCoord = aTexCoord;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 fragmentColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    fragmentColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
)glsl";

GLFWwindow *createWindow(const GLint &width, const GLint &height, const string &title) {
    GLFWwindow *window;
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int generateSquareVertexArray() {
    vector<float> positions = getSquarePositions();
    vector<unsigned int> indices = getSquareIndicesPositions();

    unsigned int vertexArray;
    unsigned int vertexBuffer;
    unsigned int elementBuffer;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertexArray;
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = createWindow(WIDTH, HEIGHT, getTitle());
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int squareVertexArray = generateSquareVertexArray();

    unsigned int shader = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shader);

    unsigned int texture1 = bindTextureRGB(
            "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/img/container.jpg");
    unsigned int texture2 = bindTextureRGBA(
            "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/img/awesomeface.png");

    int texture1UniformLocation = glGetUniformLocation(shader, "texture1");
    glUniform1i(texture1UniformLocation, 0);

    int texture2UniformLocation = glGetUniformLocation(shader, "texture2");
    glUniform1i(texture2UniformLocation, 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(squareVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

vector<float> getSquarePositions() {
    vector<float> positions = {
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    return positions;
}

vector<unsigned int> getSquareIndicesPositions() {
    vector<unsigned int> positions = {
            0, 1, 2,
            0, 2, 3
    };

    return positions;
}

string getTitle() {
    string title = "Texture Box";

    return title;
}
