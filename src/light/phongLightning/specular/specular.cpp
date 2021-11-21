#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include "cameraApi.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace std;

struct GraphicContext {
    GLFWwindow *window;
};

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

string getTitle();

vector<float> getCubePositions();

vector<float> getPlatePositions();

vector<unsigned int> getPlateIndices();

string getDefaultVertexShaderPath();

string getDefaultFragmentShaderPath();

string getLightSourceFragmentShaderPath();

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void calculateFrames() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

unsigned int generateCubeVertexArray() {
    vector<float> positions = getCubePositions();

    unsigned int vertexArray;
    unsigned int vertexBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    unsigned long vertexBufferSize = positions.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long> (vertexBufferSize), &positions.front(), GL_STATIC_DRAW);

    int strideSize = 6 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideSize, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideSize, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertexArray;
}

unsigned int generatePlateVertexArray() {
    vector<float> positions = getPlatePositions();
    vector<unsigned int> indices = getPlateIndices();

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
    ShaderProgram lightSourceShaderProgram = ShaderProgram::createShaderProgramFromFiles(getDefaultVertexShaderPath(),
                                                                                         getLightSourceFragmentShaderPath());
    ShaderProgram defaultShaderProgram = ShaderProgram::createShaderProgramFromFiles(getDefaultVertexShaderPath(),
                                                                                     getDefaultFragmentShaderPath());

    unsigned int cubeVertexArray = generateCubeVertexArray();
    unsigned int plateVertexArray = generatePlateVertexArray();

    glm::vec3 objColor(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(context.window)) {
        calculateFrames();
        processInput(context.window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing plate
        defaultShaderProgram.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f,
                                                100.0f);
        defaultShaderProgram.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        defaultShaderProgram.setMat4("view", view);

        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        defaultShaderProgram.setMat4("model", model);

        defaultShaderProgram.setVec3("objectColor", objColor);
        defaultShaderProgram.setVec3("lightColor", lightColor);
        defaultShaderProgram.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
        defaultShaderProgram.setVec3("viewPos", camera.Position);

        glBindVertexArray(plateVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Drawing of light cube
        lightSourceShaderProgram.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        defaultShaderProgram.setMat4("projection", projection);

        view = camera.GetViewMatrix();
        defaultShaderProgram.setMat4("view", view);

        model = glm::mat4(1.0f);
        defaultShaderProgram.setMat4("model", model);

        lightSourceShaderProgram.setVec3("lightColor", lightColor);

        glBindVertexArray(cubeVertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    GraphicContext graphicContext{window};
    graphicLogic(graphicContext);

    glfwTerminate();
    return 0;
}

vector<float> getCubePositions() {
    vector<float> positions = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    return positions;
}

vector<float> getPlatePositions() {
    vector<float> positions = {
            5.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f,
            5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f,
            -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f,
            -5.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f
    };

    return positions;
}

vector<unsigned int> getPlateIndices() {
    vector<unsigned int> indices = {
            0, 1, 3,
            1, 2, 3
    };
    return indices;
}

string getTitle() {
    string title = "Blinking Colored Cubes";

    return title;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

string getDefaultVertexShaderPath() {
    return "/home/mlgmag/CLionProjects/graphicsLabs/src/light/phongLightning/specular/shaders/defaultVertex.shader";
}

string getDefaultFragmentShaderPath() {
    return "/home/mlgmag/CLionProjects/graphicsLabs/src/light/phongLightning/specular/shaders/defaultFragment.shader";

}

string getLightSourceFragmentShaderPath() {
    return "/home/mlgmag/CLionProjects/graphicsLabs/src/light/phongLightning/specular/shaders/lightSourceFragment.shader";
}
