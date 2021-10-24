// Main feature of Vertex Array Object is the idea of reusing them. \
So that you do not need to reconfigure vertex buffer object (VBO) \
every time you want to draw something.


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

string getTitle();

vector<float> getTrianglePositions();

vector<float> getSquarePositions();

vector<float> getLeftTopTrianglePositions();

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
   gl_Position = position;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

void main(){
    color = vec4(1.0, 0.0, 0.0, 1.0);
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

unsigned int generateTriangleVertexArray() {
    vector<float> positions = getTrianglePositions();

    unsigned int triangleVertexArray;
    unsigned int triangleVertexBuffer;
    glGenVertexArrays(1, &triangleVertexArray);
    glGenBuffers(1, &triangleVertexBuffer);

    glBindVertexArray(triangleVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return triangleVertexArray;
}

unsigned int generateSquareVertexArray() {
    vector<float> positions = getSquarePositions();

    unsigned int squareVertexArray;
    unsigned int squareVertexBuffer;
    glGenVertexArrays(1, &squareVertexArray);
    glGenBuffers(1, &squareVertexBuffer);

    glBindVertexArray(squareVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return squareVertexArray;
}

unsigned int generateLeftTopTriangle() {
    vector<float> positions = getLeftTopTrianglePositions();

    unsigned int leftTopTriangleVertexArray;
    unsigned int leftTopTriangleVertexBuffer;

    glGenVertexArrays(1, &leftTopTriangleVertexArray);
    glGenBuffers(1, &leftTopTriangleVertexBuffer);

    glBindVertexArray(leftTopTriangleVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, leftTopTriangleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return leftTopTriangleVertexArray;
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

    unsigned int triangleVertexArray = generateTriangleVertexArray();
    unsigned int squareVertexArray = generateSquareVertexArray();
    unsigned int leftTopTriangleVertexArray = generateLeftTopTriangle();

    unsigned int shader = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.647f, 0.0f, 1.0f);
        processInput(window);

        glBindVertexArray(triangleVertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(squareVertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(leftTopTriangleVertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

vector<float> getTrianglePositions() {
    vector<float> positions = {
            -0.5f, -0.5f,
            0.0f, 0.5f,
            0.5f, -0.5f
    };

    return positions;
}

vector<float> getSquarePositions() {
    vector<float> positions = {
            -1.0f, -1.0f,
            -0.5f, -1.0f,
            -0.5f, -0.5f,

            -1.0f, -1.0f,
            -0.5f, -0.5f,
            -1.0f, -0.5f
    };

    return positions;
}

vector<float> getLeftTopTrianglePositions() {
    vector<float> positions = {
            -1.0f, 0.5f,
            -0.5f, 1.0f,
            -1.0f, 1.0f,
    };

    return positions;
}

string getTitle() {
    string title = "Vertex Array Object Practice";

    return title;
}
