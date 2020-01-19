#pragma once

#include "../texture.h"
#include "../shader.h"
#include "../mesh.h"
#include "../FPScamera.h"

class RotatingCube
{
    glm::mat4 _worldTransform;
    float _blendFactor;

    Shader<uniforms::Basic> _shader;
    Texture _texWood, _texSmiley;
    Mesh _mesh;

public:
    RotatingCube();

    void Update(double currentTime);
    void Render(const FPSCamera& camera) const;
};