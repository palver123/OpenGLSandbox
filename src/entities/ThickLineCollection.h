#pragma once

#include "../shader.h"
#include "../lineMesh.h"
#include "../FPScamera.h"

class ThickLineCollection
{
    glm::mat4 _worldTransform;
    glm::vec3 _color;

    Shader<uniforms::ThickLines> _shader;
    LineMesh _mesh;

public:
    ThickLineCollection(const LineMesh& mesh);

    void Update(double currentTime);
    void Render(const FPSCamera& camera) const;

    float thicknessInPixel = 10.0f;
};