#include "GLLineCollection.h"
#include <glad\glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLLineCollection::GLLineCollection(const LineMesh& mesh) :
    _shader{ "passthroughVS.glsl", "colorFS.glsl" },
    _mesh(mesh),
    color(0.38f, 0.84f, 0.8f)
{
}

void GLLineCollection::Update(double currentTime) { }

void GLLineCollection::Render(const FPSCamera& camera) const
{
    const auto viewTransform = camera.ViewMatrix();
    const auto projTransform = camera.ProjectionMatrix();
    const GLfloat* shaderParams[] = {
        glm::value_ptr(worldTransform),
        glm::value_ptr(viewTransform),
        glm::value_ptr(projTransform),
        glm::value_ptr(color),
    };

    _shader.Use(shaderParams);
    glLineWidth(thicknessInPixel);
    _mesh.DrawMe();
}
