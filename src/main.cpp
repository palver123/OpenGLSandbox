#define LINE_MESH_PROTO

#ifdef LINE_MESH_PROTO
#include "entities\ThickLineStrip.h"
#include "entities/GLLineCollection.h"
#else
#include "entities\RotatingCube.h"
#endif
#include "window.h"
#include <iostream>
#include <memory>


constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

#ifdef LINE_MESH_PROTO
std::unique_ptr<ThickLineStrip> entity;
std::unique_ptr<GLLineCollection> linesGL;
std::unique_ptr<GLLineCollection> linesGL2;
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
    const std::vector<GLfloat> coordinates{
        0.0f, -1.0f, 0.0f,
        0.15f, -0.8f, 0.0f,
        -0.2f, -0.3f, 0.0f,
        0.23f, -0.1f, 0.0f,
        -0.05f, 0.4f, 0.0f,
        -0.2f, 0.9f, 0.0f,
        -0.25f, 1.0f, 0.0f
    };
    const auto lineMesh = primitives::ToLineStripMesh(coordinates);
    const LineMesh strip(coordinates, true);
    const auto lines = LineMesh::SplitPolyline(coordinates);
    entity = std::make_unique<ThickLineStrip>(lineMesh);
    linesGL = std::make_unique<GLLineCollection>(strip);
    linesGL->worldTransform = glm::translate(glm::mat4(1), glm::vec3(0.5f, 0, 0));
    linesGL2 = std::make_unique<GLLineCollection>(lines);
    linesGL2->color = glm::vec3(0.45f, 0.55f, 0.78f);
    linesGL2->worldTransform = glm::translate(glm::mat4(1), glm::vec3(-0.5f, 0, 0));
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
    linesGL->Update(currentTime);
    linesGL2->Update(currentTime);
}

void Render(const MyWindow& window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    entity->Render(window.Camera());
    linesGL->Render(window.Camera());
    linesGL2->Render(window.Camera());
    glfwSwapBuffers(window.glWindow);
}
