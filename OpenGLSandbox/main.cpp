#include "shader.h"
#include "texture.h"
#include "mesh.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

glm::f32 rotAngle = 0;

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

    constexpr glm::f32 step = 1.0f;
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_G:
        rotAngle -= step;
        break;
    case GLFW_KEY_H:
        rotAngle += step;
        break;
    default:
        break;
    }
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
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, OnKeyEvent);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    const auto mesh = primitives::Cube();

    Texture texWood{ "container.jpg" };
    Texture texSmiley{ "awesomeface.png", 1 };

    Shader shader{ "basicVS.glsl", "basicFS.glsl" };
    texWood.Init(shader.Program, "ourTexture1");
    texSmiley.Init(shader.Program, "ourTexture2");
    GLint locOfShaderVariable = glGetUniformLocation(shader.Program, "texBlendFactor");
    GLint locOfShaderVariable2 = glGetUniformLocation(shader.Program, "model");
    GLint locOfShaderVariable3 = glGetUniformLocation(shader.Program, "view");
    GLint locOfShaderVariable4 = glGetUniformLocation(shader.Program, "projection");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        shader.Use();

        GLdouble currentTime = glfwGetTime();
        const auto blendFactor = sin(currentTime) / 4 + 0.25;
        glUniform1f(locOfShaderVariable, static_cast<float>(blendFactor));

        const auto worldTransform = glm::rotate(glm::mat4(1.f), glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(1.0f, 0.0f, 0.0f));;
        const auto viewTransform = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f));
        const auto projTransform = glm::perspective(45.0f, 4 / 3.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(locOfShaderVariable2, 1, GL_FALSE, glm::value_ptr(worldTransform));
        glUniformMatrix4fv(locOfShaderVariable3, 1, GL_FALSE, glm::value_ptr(viewTransform));
        glUniformMatrix4fv(locOfShaderVariable4, 1, GL_FALSE, glm::value_ptr(projTransform));

        mesh.Bind();
        texWood.Bind();
        texSmiley.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}