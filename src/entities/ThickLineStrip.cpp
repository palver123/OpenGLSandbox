#include "ThickLineStrip.h"
#include <glad\glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ThickLineStrip::ThickLineStrip(const ThickLineMesh& mesh) :
    _shader{ "lineStripsVS.glsl", "colorFS.glsl" },
    _mesh(mesh),
    _color(0.78f, 0.14f, 0.0f)
{
}

void ThickLineStrip::Update(double currentTime)
{
    _worldTransform = glm::mat4(1.f);
}

void ThickLineStrip::Render(const FPSCamera& camera) const
{
    const auto viewTransform = camera.ViewMatrix();
    const auto projTransform = camera.ProjectionMatrix();
    const GLfloat* shaderParams[] = {
        glm::value_ptr(_worldTransform),
        glm::value_ptr(viewTransform),
        glm::value_ptr(projTransform),
        glm::value_ptr(_color),
        &thicknessInPixel,
        &camera.aspectRatio
    };

    _shader.Use(shaderParams);
    _mesh.DrawMe();
}