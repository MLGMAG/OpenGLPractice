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

vector<float> getSquarePositions();

vector<unsigned int> getSquareIndices();

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
   gl_Position = position;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

layout(location = 0) out vec4 fragmentColor;

uniform vec4 color;

void main(){
    fragmentColor = color;
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
    vector<unsigned int> indices = getSquareIndices();

    unsigned int squareVertexArray;
    unsigned int squareVertexBuffer;
    unsigned int squareElementBuffer;
    glGenVertexArrays(1, &squareVertexArray);
    glGenBuffers(1, &squareVertexBuffer);
    glGenBuffers(1, &squareElementBuffer);

    glBindVertexArray(squareVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

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
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        double timeValue = glfwGetTime();
        double greenValue = sin(timeValue - (M_PI / 2)) / 2.0f + 0.5f;
        double redValue = cos(timeValue) / 2.0f + 0.5f;
        glUniform4f(vertexColorLocation, (float) redValue, (float) greenValue, 0.0f, 1.0f);

        glBindVertexArray(squareVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

vector<float> getSquarePositions() {
    vector<float> positions = {
            -0.5f, 0.5f,
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f
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
    string title = "Uniform Practice";

    return title;
}
