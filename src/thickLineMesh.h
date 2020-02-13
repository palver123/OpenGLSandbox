#pragma once

#include <glad\glad.h>
#include <vector>

class ThickLineMesh {
    GLuint _vertexBuffer, _indexBuffer;
    GLsizei _elementCount;

    void Bind() const;

public:
    GLuint ID;

    ThickLineMesh(const std::vector<GLfloat>& vertices);
    ~ThickLineMesh();

    void DrawMe() const;
};

namespace primitives
{
    ThickLineMesh ToLineMesh(const std::vector<GLfloat>& coordinates);
}