#include "shader.h"
#include "texture.h"
#include "mesh.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

static constexpr int kWindowWidth = 800;
static constexpr int kWindowHeight = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0;    // Time between current frame and last frame
double lastFrame = 0.0;   // Time of last frame
double lastX = 400, lastY = 300;
double yaw = -90;
double pitch = 0;
bool firstMouse = true;

GLFWwindow* CreateMainWindow(int width, int height);
void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    glfwInit();

    const auto window = CreateMainWindow(kWindowWidth, kWindowHeight);
    if (nullptr == window)
    {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, OnKeyEvent);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    const auto mesh = primitives::Cube();

    Texture texWood{ "container.jpg" };
    Texture texSmiley{ "awesomeface.png", 1, true };

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

        auto currentTime = glfwGetTime();
        deltaTime = (float)(currentTime - lastFrame);
        lastFrame = currentTime;
        ProcessInput(window);

        const auto blendFactor = sin(currentTime) / 4 + 0.25;
        glUniform1f(locOfShaderVariable, static_cast<float>(blendFactor));

        const auto worldTransform = glm::rotate(glm::mat4(1.f), glm::radians((GLfloat)(currentTime * 50.0)), glm::vec3(1.0f, 0.0f, 0.0f));;
        const auto viewTransform = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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

GLFWwindow* CreateMainWindow(int width, int height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    const auto window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    return window;
}

void ProcessInput(GLFWwindow* window)
{
    const auto cameraSpeed = 2.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    constexpr auto sensitivity = 0.075;
    const auto xoffset = (xpos - lastX) * sensitivity;
    const auto yoffset = (lastY - ypos) * sensitivity; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}