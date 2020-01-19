#include "RotatingCube.h"
#include <glad\glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

RotatingCube::RotatingCube() :
    _texWood{ "container.jpg" },
    _texSmiley{ "awesomeface.png", 1, true },
    _shader{ "basicVS.glsl", "basicFS.glsl" },
    _mesh(primitives::Cube())
{
    _texWood.Init(_shader.Program, "ourTexture1");
    _texSmiley.Init(_shader.Program, "ourTexture2");
}

void RotatingCube::Update(double currentTime)
{
    _blendFactor = static_cast<float>(sin(currentTime) / 4 + 0.25);
    _worldTransform = glm::rotate(glm::mat4(1.f), glm::radians((GLfloat)(currentTime * 50.0)), glm::vec3(1.0f, 0.0f, 0.0f));
}

void RotatingCube::Render(const FPSCamera& camera) const
{
    const auto viewTransform = camera.ViewMatrix();
    const auto projTransform = camera.ProjectionMatrix();
    const GLfloat* shaderParams[] = {
        glm::value_ptr(_worldTransform),
        glm::value_ptr(viewTransform),
        glm::value_ptr(projTransform),
        &_blendFactor
    };

    _shader.Use(shaderParams);
    _texWood.Bind();
    _texSmiley.Bind();
    _mesh.DrawMe();
}
