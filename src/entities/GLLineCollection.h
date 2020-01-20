#pragma once

#include "../shader.h"
#include "../lineMesh2.h"
#include "../FPScamera.h"

class GLLineCollection
{
    Shader<uniforms::JustColor> _shader;
    LineMesh2 _mesh;

public:
    GLLineCollection(const LineMesh2& mesh);

    void Update(double currentTime);
    void Render(const FPSCamera& camera) const;

    float thicknessInPixel = 10.0f;
    glm::vec3 color;
    glm::mat4 worldTransform;
};