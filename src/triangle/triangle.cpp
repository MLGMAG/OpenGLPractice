#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <string>

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        std::cout << "Failed to compile " + shaderType + " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;

    std::vector<float> positions = {
            -0.5f, -0.5f,
            0.0f, 0.5f,
            0.5f, -0.5f
    };

    /* Initialize buffer */
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    /* Select current buffer */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /* Set data to buffer */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions.front(), GL_STATIC_DRAW);

    /* specify type of data */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    std::string vertexShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) in vec4 position;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    gl_Position = position;\n"
                               "}\n";

    std::string fragmentShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) out vec4 color;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                               "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
