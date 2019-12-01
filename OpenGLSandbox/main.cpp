#include "shader.h"
#include "texture.h"
#include "mesh.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

glm::mat4::value_type translationZ = 0;

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
    if (action != GLFW_PRESS)
        return;

    constexpr glm::mat4::value_type step = 0.033f;
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_G:
        translationZ -= step;
        break;
    case GLFW_KEY_H:
        translationZ += step;
        break;
    default:
        break;
    }
}

Mesh CreateRectangle()
{
    const GLfloat half = 0.5f;
    const GLfloat tex = 1.0;
    vector<GLfloat> vertices = {
         half,  half, 0, tex, tex,  // Top Right
         half, -half, 0, tex, 0,    // Bottom Right
        -half, -half, 0, 0, 0,      // Bottom Left
        -half,  half, 0, 0, tex     // Top Left
    };
    vector<GLuint> indices = { // Note that we start from 0!
        0, 1, 3, // First Triangle
        3, 2, 1 // Second Triangle
    };

    return Mesh{ vertices, indices };
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

    const auto mesh = CreateRectangle();

    Texture texWood{ "container.jpg" };
    Texture texSmiley{ "awesomeface.png", 1 };

    Shader shader{ "basicVS.glsl", "basicFS.glsl" };
    texWood.Init(shader.Program, "ourTexture1");
    texSmiley.Init(shader.Program, "ourTexture2");
    GLint locOfShaderVariable = glGetUniformLocation(shader.Program, "texBlendFactor");
    GLint locOfShaderVariable2 = glGetUniformLocation(shader.Program, "worldTransform");
    glm::mat4 worldTransform = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, translationZ));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        GLdouble currentTime = glfwGetTime();
        const auto blendFactor = sin(currentTime) / 4 + 0.25;
        shader.Use();
        glUniform1f(locOfShaderVariable, static_cast<float>(blendFactor));
        worldTransform[3].x = translationZ;
        glUniformMatrix4fv(locOfShaderVariable2, 1, GL_FALSE, glm::value_ptr(worldTransform));

        mesh.Use();
        texWood.Use();
        texSmiley.Use();

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}