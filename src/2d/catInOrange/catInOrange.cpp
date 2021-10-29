#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderApi.h"
#include <vector>
#include <string>

using namespace std;

GLint getWidth();

GLint getHeight();

string getTitle();

string getVertexShaderSourceFilepath();

string getFragmentShaderSourceFilepath();

vector<float> getPositions();

vector<unsigned int> getIndices();

GLFWwindow *createWindow(const GLint &width, const GLint &height, const string &title) {
    GLFWwindow *window;
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    return window;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void initializeBuffer(unsigned int &vertexArrayBuffer, vector<float> positions) {
    glGenBuffers(1, &vertexArrayBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * 2 * sizeof(float), &positions.front(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
}

void updateBuffer(vector<float> &positions, const float &xInc, const float &yInc) {
    for (size_t i = 0; i < positions.size(); i++) {
        if (i % 2 == 0) {
            positions[i] += xInc;
        } else {
            positions[i] += yInc;
        }

        if (positions[i] > 1) {
            positions[i] -= 2;
        }

        if (positions[i] < -1) {
            positions[i] += 2;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, positions.size() * 2 * sizeof(float), &positions.front(), GL_DYNAMIC_DRAW);
}

void processMovements(GLFWwindow *window, vector<float> &positions) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        updateBuffer(positions, 0.0f, 0.005f);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        updateBuffer(positions, 0.005f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        updateBuffer(positions, 0.0f, -0.005f);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        updateBuffer(positions, -0.005f, 0.0f);
    }
}

void initializeIndexBuffer(unsigned int &indexBuffer, vector<unsigned int> indices) {
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_DYNAMIC_DRAW);
}

void setBackGround() {
    glClearColor(1.0f, 0.4745f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void displayTriangles(const unsigned int &vertexArrayBuffer) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, nullptr);
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = createWindow(getWidth(), getHeight(), getTitle());
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int buffer;
    vector<float> positions = getPositions();
    initializeBuffer(buffer, positions);

    unsigned int indexBuffer;
    initializeIndexBuffer(indexBuffer, getIndices());

    const string vertexShaderPath = getVertexShaderSourceFilepath();
    const string fragmentShaderPath = getFragmentShaderSourceFilepath();

    unsigned int shader = ShaderUtils::CreateShaderFromFiles(vertexShaderPath, fragmentShaderPath);
    glUseProgram(shader);

    int colorLocation = glGetUniformLocation(shader, "u_Color");
    if (colorLocation == -1) {
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        processMovements(window, positions);

        setBackGround();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform4f(colorLocation, 0.0f, 0.6727f, 0.0f, 1.0f);
        displayTriangles(buffer);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(3.0f);
        glUniform4f(colorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        displayTriangles(buffer);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

GLint getWidth() {
    static GLint width = 640;
    return width;
}

GLint getHeight() {
    static GLint height = 480;
    return height;
}

string getTitle() {
    static string title = "Cat in orange";
    return title;
}

vector<float> getPositions() {
    static std::vector<float> positions = {
            0.0f, 0.0f,
            0.25f, -0.25f,
            0.5f, -0.5f,
            0.5f, 0.0f,
            0.7f, 0.05f,
            0.8f, 0.25f,
            0.6f, 0.2f,
            -0.5f, 0.0f,
            -0.5f, 0.1f,
            -0.5f, 0.3f,
            -0.55f, 0.25f,
            -0.7f, 0.45f,
            -0.7f, 0.1f,
            -0.7f, -0.1f,
            -0.5f, -0.1f,
            -0.5f, -0.5f,
            -0.25f, -0.25f,
    };
    return positions;
}

vector<unsigned int> getIndices() {
    static vector<unsigned int> indices = {
            0, 2, 3,
            3, 4, 6,
            4, 5, 6,
            0, 16, 1,
            7, 15, 0,
            13, 14, 8,
            12, 13, 8,
            12, 8, 9,
            11, 12, 10
    };
    return indices;
}

string getVertexShaderSourceFilepath() {
    static string filePath = "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/shaders/vertex.shader";
    return filePath;
}

string getFragmentShaderSourceFilepath() {
    static string filePath = "/home/mlgmag/CLionProjects/graphicsLabs/src/resources/shaders/fragment.shader";
    return filePath;
}
