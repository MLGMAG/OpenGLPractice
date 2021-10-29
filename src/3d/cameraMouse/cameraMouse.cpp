#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>
#include "texturesApi.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const GLint WIDTH = 640;
const GLint HEIGHT = 480;

double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;

struct Camera {
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float yaw;
    float pitch;
};

Camera globalCamera;

string getTitle();

vector<float> getCubePositions();

vector<glm::vec3> getModelPositions();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const string VERTEX_SHADER = R"glsl(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)glsl";

const string FRAGMENT_SHADER = R"glsl(
#version 330 core

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

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    auto xoffset = static_cast<float> (xpos - lastX);
    auto yoffset = static_cast<float>(lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    globalCamera.yaw += xoffset;
    globalCamera.pitch += yoffset;

    if (globalCamera.pitch > 89.0f)
        globalCamera.pitch = 89.0f;
    if (globalCamera.pitch < -89.0f)
        globalCamera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(globalCamera.yaw)) * cos(glm::radians(globalCamera.pitch));
    direction.y = sin(glm::radians(globalCamera.pitch));
    direction.z = sin(glm::radians(globalCamera.yaw)) * cos(glm::radians(globalCamera.pitch));
    globalCamera.cameraFront = glm::normalize(direction);
}

void processCameraInput(GLFWwindow *window, Camera &currentCamera) {
    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 cameraPosValue = cameraSpeed * currentCamera.cameraFront;
        currentCamera.cameraPos += cameraPosValue;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 cameraPosValue = cameraSpeed * currentCamera.cameraFront;
        currentCamera.cameraPos -= cameraPosValue;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 cameraFrontValue = glm::normalize(glm::cross(currentCamera.cameraFront, currentCamera.cameraUp));
        glm::vec3 cameraPosValue = cameraSpeed * cameraFrontValue;
        currentCamera.cameraPos -= cameraPosValue;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 cameraFrontValue = glm::normalize(glm::cross(currentCamera.cameraFront, currentCamera.cameraUp));
        glm::vec3 cameraPosValue = cameraSpeed * cameraFrontValue;
        currentCamera.cameraPos += cameraPosValue;
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

glm::mat4 generateLookAtMatrix(const Camera &camera) {
    return glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
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

    int strideSize = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideSize, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideSize, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertexArray;
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = createWindow(WIDTH, HEIGHT, getTitle());
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    unsigned int cubeVertexArray = generateCubeVertexArray();

    unsigned int shader = ShaderUtils::CreateShader(VERTEX_SHADER, FRAGMENT_SHADER);
    glUseProgram(shader);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int texture1 = bindTextureRGB(
            "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/img/container.jpg");
    unsigned int texture2 = bindTextureRGBA(
            "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/img/awesomeface.png");

    int texture1UniformLocation = glGetUniformLocation(shader, "texture1");
    glUniform1i(texture1UniformLocation, 0);

    int texture2UniformLocation = glGetUniformLocation(shader, "texture2");
    glUniform1i(texture2UniformLocation, 1);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    int projectionLocation = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLocation = glGetUniformLocation(shader, "model");
    int viewLocation = glGetUniformLocation(shader, "view");

    glEnable(GL_DEPTH_TEST);

    vector<glm::vec3> modelPositions = getModelPositions();

    globalCamera = {
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            -90.0f,
            0.0f
    };

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        auto currentFrame = static_cast<float> (glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processCameraInput(window, globalCamera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(cubeVertexArray);
        for (size_t i = 0; i < modelPositions.size(); i++) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, modelPositions[i]);
            float angle = 20.0f * static_cast<float>(i + 1);
            model = glm::rotate(model, (float) glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::mat4 view = generateLookAtMatrix(globalCamera);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

// https://community.khronos.org/t/cube-with-indices/105329
vector<float> getCubePositions() {
    vector<float> positions = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    return positions;
}

vector<glm::vec3> getModelPositions() {
    vector<glm::vec3> positions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    return positions;
}

string getTitle() {
    string title = "Camera Mouse";

    return title;
}
