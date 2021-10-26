#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

string getTitle();

vector<float> getSquareData();

vector<unsigned int> getSquareIndices();

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

void main() {
    gl_Position = position;
    vertexColor = color;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

in vec3 vertexColor;
out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(vertexColor, 1.0);
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
    vector<float> positions = getSquareData();
    vector<unsigned int> indices = getSquareIndices();

    unsigned int squareVertexArray;
    unsigned int squareVertexBuffer;
    unsigned int squareElementBuffer;
    glGenVertexArrays(1, &squareVertexArray);
    glGenBuffers(1, &squareVertexBuffer);
    glGenBuffers(1, &squareElementBuffer);

    glBindVertexArray(squareVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return squareVertexArray;
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

    unsigned int squareVertexArray = generateSquareVertexArray();
    unsigned int shaderProgram = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glBindVertexArray(squareVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

vector<float> getSquareData() {
    vector<float> positions = {
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    };

    return positions;
}

vector<unsigned int> getSquareIndices() {
    vector<unsigned int> positions = {
            0, 1, 2,
            0, 2, 3
    };

    return positions;
}

string getTitle() {
    string title = "Layout Practice";

    return title;
}
