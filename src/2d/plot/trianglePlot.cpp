#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;
const string TITLE = "First Lab";

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
   gl_Position = position;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

out vec4 color;

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

float functionToDisplay(float x) {
    return x;
}

template<class Function>
vector<float> generateCoords(float xStart, float xEnd, float step, Function func) {
    vector<float> data;

    for (float i = xStart; i <= xEnd;) {
        float y = func(i);
        data.push_back(i);
        data.push_back(y);
        data.push_back(0);

        i += step;
    }

    return data;
}

vector<float> generateLowerYCoord(vector<float> data) {
    vector<float> result;

    for (size_t i = 0; i < data.size(); i += 3) {
        result.push_back(data[i]);
        result.push_back(data[i + 1]);
        result.push_back(data[i + 2]);

        result.push_back(data[i]);
        result.push_back(0);
        result.push_back(0);
    }

    return result;
}

void scaleCoords(vector<float> &data, float xScale, float yScale, float zScale) {
    for (size_t i = 0; i < data.size(); i += 3) {
        data[i] /= xScale;
        data[i + 1] /= yScale;
        data[i + 2] /= zScale;
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = createWindow(WIDTH, HEIGHT, TITLE);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    std::vector<float> coords = generateCoords(-10.0f, 10.0f, 0.5f, functionToDisplay);
    std::vector<float> positions = generateLowerYCoord(coords);
    scaleCoords(positions, 10, 10, 1);

    std::vector<unsigned int> indices;

    unsigned long upperBound = (positions.size() / 3) - 2;
    for (unsigned int i = 1; i < upperBound; i += 2) {
        indices.push_back(i - 1);
        indices.push_back(i);
        indices.push_back(i + 1);

        indices.push_back(i + 1);
        indices.push_back(i);
        indices.push_back(i + 2);
    }

    unsigned int vertexArray;
    unsigned int arrayBuffer;
    unsigned int elementBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &arrayBuffer);
    glGenBuffers(1, &elementBuffer);

    glBindVertexArray(vertexArray);

    unsigned long arrayBufferSize = positions.size() * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long> (arrayBufferSize), &positions.front(), GL_STATIC_DRAW);

    unsigned long elementBufferSize = indices.size() * sizeof(unsigned int);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(elementBufferSize), &indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int shader = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glBindVertexArray(vertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, static_cast<int>(elementBufferSize / 3), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
