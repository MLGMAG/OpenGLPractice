#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;
const string TITLE = "Plot";

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
   gl_Position = position;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

out vec4 FragColor;

uniform vec3 aColor;

void main(){
    FragColor = vec4(aColor, 1.0);
}
)glsl";

vector<float> getMetricsPositions();

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

void scaleCoords(vector<float> &data, float xScale, float yScale) {
    for (size_t i = 0; i < data.size(); i += 2) {
        data[i] /= xScale;
        data[i + 1] /= yScale;
    }
}

template<class Function>
vector<float> generateCoords(float xStart, float xEnd, float step, Function func) {
    vector<float> data;
    float yAbsMax = abs(func(xStart));
    float xAbsMax = max(abs(xStart), abs(xEnd));

    for (float i = xStart; i <= xEnd;) {
        float y = func(i);
        data.push_back(i);
        data.push_back(y);

        if (float yAbs = abs(y); yAbs > yAbsMax) {
            yAbsMax = yAbs;
        }

        i += step;
    }

    scaleCoords(data, xAbsMax, yAbsMax);

    return data;
}

unsigned int createMetricVertexArray() {
    vector<float> positions = getMetricsPositions();

    unsigned int vertexArray;
    unsigned int arrayBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &arrayBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);

    unsigned long vertexBufferSize = positions.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long> (vertexBufferSize), &positions.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertexArray;
}

unsigned int createGraphVertexArray(vector<float> coords) {
    unsigned int vertexArray;
    unsigned int arrayBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &arrayBuffer);

    glBindVertexArray(vertexArray);

    unsigned long arrayBufferSize = coords.size() * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long> (arrayBufferSize), &coords.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertexArray;
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

    auto functionToDisplay = [](float x) { return x * x * x; };

    unsigned int graphMetricVertexArray = createMetricVertexArray();

    vector<float> coords = generateCoords(-100.0f, 100.0f, 10.0f, functionToDisplay);
    unsigned int graphVertexArray = createGraphVertexArray(coords);

    unsigned int shader = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shader);

    int colorUniformLocation = glGetUniformLocation(shader, "aColor");
    glUniform3f(colorUniformLocation, 0.0f, 1.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glBindVertexArray(graphMetricVertexArray);
        glUniform3f(colorUniformLocation, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 0, 12);

        glBindVertexArray(graphVertexArray);
        glUniform3f(colorUniformLocation, 1.0f, 0.0f, 0.0f);
        auto linesToDisplay = static_cast<int>(coords.size() / 2);
        glDrawArrays(GL_LINE_STRIP, 0, linesToDisplay);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

vector<float> getMetricsPositions() {
    vector<float> positions = {
            0.0f, 1.0f,
            0.0f, -1.0f,

            0.0f, 1.0f,
            0.025f, 0.9f,

            0.0f, 1.0f,
            -0.025f, 0.9f,

            -1.0f, 0.0f,
            1.0f, 0.0f,

            1.0f, 0.0f,
            0.9f, 0.03f,

            1.0f, 0.0f,
            0.9f, -0.03f
    };

    return positions;
}
