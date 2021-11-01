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

struct GraphData {
    unsigned int vertexArray;
    int vertexToDraw;
};

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const GLint WIDTH = 2 * 640;
const GLint HEIGHT = 2 * 480;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

glm::mat4 projection;

string getTitle();

vector<float> getCubePositions();

vector<unsigned int> getCubeIndices();

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        projection = glm::ortho(-(WIDTH / 256.0f), WIDTH / 256.0f,
                                -(HEIGHT / 256.0f), (HEIGHT / 256.0f),
                                -1000.0f, 1000.0f);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f,
                                      100.0f);
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

GraphData generateGraph1VertexArray() {
    vector<float> positions;

    auto func = [](float x, float y) {
        return x * x + y * y;
    };
    float start = -3.0f;
    float end = 3.0f;
    float step = 0.1f;

    for (float x = start; x < end;) {
        for (float y = start; y < end;) {
            positions.push_back(x);
            positions.push_back(func(x, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x);
            positions.push_back(func(x, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x);
            positions.push_back(func(x, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            y += step;
        }
        x += step;
    }

    int vertexToDraw = pow((abs(start) + abs(end)) / step, 2) * 6;

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

    return {vertexArray, vertexToDraw};
}

GraphData generateGraph2VertexArray() {
    vector<float> positions;

    auto func = [](float x, float y) {
        if (x < 0) {
            x = 0.0f;
        }

        if (y < 0) {
            y = 0.0f;
        }

        return sqrt(x) + sqrt(y);
    };

    float start = -4.0f;
    float end = 4.0f;
    float step = 0.1f;

    for (float x = start; x < end;) {
        for (float y = start; y < end;) {
            positions.push_back(x);
            positions.push_back(func(x, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x);
            positions.push_back(func(x, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x);
            positions.push_back(func(x, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y));
            positions.push_back(y);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            positions.push_back(x + step);
            positions.push_back(func(x + step, y + step));
            positions.push_back(y + step);
            positions.push_back(1.0f);
            positions.push_back(0.0f);
            positions.push_back(0.0f);

            y += step;
        }
        x += step;
    }

    int vertexToDraw = pow((abs(start) + abs(end)) / step, 2) * 6;

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

    return {vertexArray, vertexToDraw};
}

GraphData generateTorusVertexArray(double r = 0.07, double c = 0.15, int rSeg = 16, int cSeg = 36) {
    vector<float> positions;

    const double PI = 3.1415926535897932384626433832795;
    const double TAU = 2 * PI;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                double s = (i + k) % rSeg + 0.5;
                double t = j % (cSeg + 1);

                double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                double z = r * sin(s * TAU / rSeg);

                positions.push_back(2 * x);
                positions.push_back(2 * y);
                positions.push_back(2 * z);
                positions.push_back(0.0f);
                positions.push_back(1.0f);
                positions.push_back(0.0f);
            }
        }
    }

    int vertexCount = positions.size() / 6;

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

    return {vertexArray, vertexCount};
}

void graphicLogic(const GraphicContext &context) {
    ShaderProgram shaderProgram = ShaderProgram::createShaderProgramFromStrings(VERTEX_SHADER, FRAGMENT_SHADER);
    shaderProgram.use();

    unsigned int cubeVertexArray = generateCubeVertexArray();

    GraphData graphic1Data = generateGraph1VertexArray();
    GraphData graphic2Data = generateGraph2VertexArray();
    GraphData torusData = generateTorusVertexArray();

    projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(context.window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(context.window);

        shaderProgram.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shaderProgram.setMat4("view", view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //      Cube 1
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        glBindVertexArray(cubeVertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        //      Cube 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        glBindVertexArray(cubeVertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        //      Graph 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, -10.0f));
        shaderProgram.setMat4("model", model);
        glBindVertexArray(graphic1Data.vertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, graphic1Data.vertexToDraw);

        //      Graph 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -10.0f));
        shaderProgram.setMat4("model", model);
        glBindVertexArray(graphic2Data.vertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, graphic2Data.vertexToDraw);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shaderProgram.setMat4("model", model);
        glBindVertexArray(torusData.vertexArray);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_LINE_STRIP, 0, torusData.vertexToDraw);

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
    string title = "Lab 2";

    return title;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
