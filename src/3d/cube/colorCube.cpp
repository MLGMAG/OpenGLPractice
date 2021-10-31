#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;


struct GraphicContext {
    GLFWwindow *window;
};

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

string getTitle();

vector<float> getCubePositions();

vector<unsigned int> getCubeIndices();

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = aColor;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

in vec3 color;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(color, 1.0);
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

unsigned int generateCubeVertexArray() {
    vector<float> positions = getCubePositions();
    vector<unsigned int> indices = getCubeIndices();

    unsigned int vertexArray;
    unsigned int vertexBuffer;
    unsigned int elementBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    unsigned long vertexBufferSize = positions.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long> (vertexBufferSize), &positions.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    unsigned long elementBufferSize = indices.size() * sizeof(unsigned int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long> (elementBufferSize), &indices.front(), GL_STATIC_DRAW);

    int strideSize = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideSize, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideSize, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertexArray;
}

void graphicLogic(const GraphicContext &context) {
    unsigned int cubeVertexArray = generateCubeVertexArray();

    ShaderProgram shaderProgram = ShaderProgram::createShaderProgramFromStrings(VERTEX_SHADER, FRAGMENT_SHADER);
    shaderProgram.use();

    auto view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shaderProgram.setMat4("view", view);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shaderProgram.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(context.window)) {
        processInput(context.window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);

        glBindVertexArray(cubeVertexArray);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(context.window);

        glfwPollEvents();
    }
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

    GraphicContext graphicContext{window};
    graphicLogic(graphicContext);

    glfwTerminate();
    return 0;
}

// https://community.khronos.org/t/cube-with-indices/105329
vector<float> getCubePositions() {
    vector<float> positions = {
            // positions      // texture coords

            //front
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

            //back
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    return positions;
}

vector<unsigned int> getCubeIndices() {
    vector<unsigned int> indices = {
            // front
            0, 1, 3,
            1, 2, 3,
            // back
            4, 5, 7,
            5, 6, 7,
            // right
            0, 1, 4,
            1, 4, 5,
            // left
            2, 3, 7,
            2, 6, 7,
            // top
            0, 3, 4,
            3, 4, 7,
            // bottom
            1, 2, 5,
            2, 5, 6
    };

    return indices;
}

string getTitle() {
    string title = "Color cube";

    return title;
}
