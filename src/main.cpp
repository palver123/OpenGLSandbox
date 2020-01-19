#define LINE_MESH_PROTO

#ifdef LINE_MESH_PROTO
#include "entities\ThickLineCollection.h"
#else
#include "entities\RotatingCube.h"
#endif
#include "window.h"
#include <iostream>
#include <memory>


constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

#ifdef LINE_MESH_PROTO
std::unique_ptr<ThickLineCollection> entity;
#else
std::unique_ptr<RotatingCube> entity;
#endif

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
#ifdef LINE_MESH_PROTO
    const auto mesh = primitives::ToLineMesh({
        0.0f, -1.0f, 0.0f,
        0.15f, -0.8f, 0.0f,
        -0.2f, -0.3f, 0.0f,
        0.23f, -0.1f, 0.0f,
        -0.05f, 0.4f, 0.0f,
        -0.2f, 0.9f, 0.0f,
        -0.25f, 1.0f, 0.0f
    });
    entity = std::make_unique<ThickLineCollection>(mesh);
#else
    entity = std::make_unique<RotatingCube>();
#endif

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
    entity->Update(currentTime);
}

void Render(const MyWindow& window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    entity->Render(window.Camera());
    glfwSwapBuffers(window.glWindow);
}
