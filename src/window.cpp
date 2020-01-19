#include "window.h"

float deltaTime = 0.0f;    // Time between current frame and last frame
double lastFrame = 0.0;   // Time of last frame
bool firstMouse = true;
MyWindow* onlyInstanceInTheProgram; // :P

namespace {
    void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (action != GLFW_PRESS)
            return;

        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    void _OnCursorMoved(GLFWwindow* window, double xpos, double ypos)
    {
        onlyInstanceInTheProgram->OnCursorMoved(window, xpos, ypos);
    }
}

MyWindow::MyWindow(int width, int height):
    _lastX(width / 2), _lastY(height / 2)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glWindow = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    onlyInstanceInTheProgram = this;
}

void MyWindow::InitEventHandlers() {
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(glWindow, _OnCursorMoved);
    glfwSetKeyCallback(glWindow, OnKeyEvent);
}

void MyWindow::ProcessInput(double currentTime)
{
    deltaTime = (float)(currentTime - lastFrame);
    lastFrame = currentTime;
    const auto cameraSpeed = 2.0f * deltaTime; // adjust accordingly

    if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS)
        _camera.Walk(cameraSpeed, FPSCamera::Direction::Forward);
    if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS)
        _camera.Walk(cameraSpeed, FPSCamera::Direction::Backward);
    if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS)
        _camera.Walk(cameraSpeed, FPSCamera::Direction::Left);
    if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS)
        _camera.Walk(cameraSpeed, FPSCamera::Direction::Right);
}

void MyWindow::OnCursorMoved(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        _lastX = xpos;
        _lastY = ypos;
        firstMouse = false;
    }

    constexpr auto sensitivity = 0.075;
    const auto xoffset = (xpos - _lastX) * sensitivity;
    const auto yoffset = (_lastY - ypos) * sensitivity; // reversed since y-coordinates range from bottom to top
    _lastX = xpos;
    _lastY = ypos;

    _camera.Rotate(xoffset, yoffset);
}