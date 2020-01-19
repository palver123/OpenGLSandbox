#include "entities/RotatingCube.h"
#include "window.h"
#include <iostream>
#include <memory>

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

std::unique_ptr<RotatingCube> cube;

void Update(MyWindow& window);
void Render(const MyWindow& window);

int main()
{
    glfwInit();

    MyWindow window(kWindowWidth, kWindowHeight);
    if (nullptr == window.glWindow)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window.glWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    window.InitEventHandlers();
    cube = std::make_unique<RotatingCube>();

    while (window.IsActive())
    {
        Update(window);
        Render(window);
    }

    glfwTerminate();
    return 0;
}

void Update(MyWindow& window)
{
    glfwPollEvents();
    auto currentTime = glfwGetTime();
    window.ProcessInput(currentTime);
    cube->Update(currentTime);
}

void Render(const MyWindow& window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube->Render(window.Camera());
    glfwSwapBuffers(window.glWindow);
}
