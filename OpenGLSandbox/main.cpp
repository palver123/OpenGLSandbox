#include "shader.h"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

GLFWwindow* CreateMainWindow(int width = 800, int height = 600)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const auto window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    return window;
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint CreateRectangle()
{
    const GLfloat half = 0.5f;
    GLfloat vertices[] = {
         half,  half, 0, // Top Right
         half, -half, 0, // Bottom Right
        -half, -half, 0, // Bottom Left
        -half,  half, 0 // Top Left
    };
    GLuint indices[] = { // Note that we start from 0!
        0, 1, 3, // First Triangle
        3, 2, 1 // Second Triangle
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);

    return vao;
}

int main()
{
    glfwInit();
    
    const auto window = CreateMainWindow();
    if (nullptr == window)
    {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetKeyCallback(window, OnKeyEvent);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    const auto vao = CreateRectangle();

    Shader shader{ "basicVS.glsl", "basicFS.glsl" };
    GLint locOfShaderVariable = glGetUniformLocation(shader.Program, "ourRed");
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        GLdouble currentTime = glfwGetTime();
        const auto alpha = sin(currentTime) / 2 + 0.5;
        shader.Use();
        glUniform1f(locOfShaderVariable, static_cast<float>(alpha));
        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}