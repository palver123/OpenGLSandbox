#pragma once

#include "glews.h"
#include <vector>

class Mesh {
    GLuint vertexBuffer, indexBuffer;

public:
    GLuint ID;

    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint> indices);
    ~Mesh();

    void Use() const;
};