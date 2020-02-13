#pragma once

#include "../shader.h"
#include "../thickLineMesh.h"
#include "../FPScamera.h"

class ThickLineCollection
{
    glm::mat4 _worldTransform;
    glm::vec3 _color;

    Shader<uniforms::ThickLines> _shader;
    ThickLineMesh _mesh;

public:
    ThickLineCollection(const ThickLineMesh& mesh);

    void Update(double currentTime);
    void Render(const FPSCamera& camera) const;

    float thicknessInPixel = 10.0f;
};