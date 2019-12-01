#pragma once

#include "glews.h"
#include <vector>

class Mesh {
    GLuint vertexBuffer, indexBuffer;

public:
    GLuint ID;

    Mesh(const std::vector<GLfloat>& vertices);
    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint> indices);
    ~Mesh();

    void Bind() const;
};

namespace primitives
{
    Mesh Square(GLfloat halfSize = 0.5f);
    Mesh Cube();
}