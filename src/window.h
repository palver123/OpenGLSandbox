#pragma once

#include "FPScamera.h"
#include <GLFW/glfw3.h>

class MyWindow {
    int _width, _height;
    double _lastX, _lastY;
    FPSCamera _camera;

public:
    GLFWwindow * glWindow;

    MyWindow(int width, int height);

    void InitEventHandlers();
    void ProcessInput(double currentTime);
    void OnCursorMoved(GLFWwindow* window, double xpos, double ypos);

    bool IsActive() const { return !glfwWindowShouldClose(glWindow); }
    const FPSCamera& Camera() const { return _camera; }
};
