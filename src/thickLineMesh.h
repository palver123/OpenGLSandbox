#pragma once

#include <glad\glad.h>
#include <vector>

class ThickLineMesh {
    GLuint vertexBuffer, indexBuffer;
    GLsizei numTriangles;

    void Bind() const;

public:
    static constexpr GLsizei kVertexStride = 6;
    GLuint ID;

    ThickLineMesh(const std::vector<GLfloat>& vertices);
    ~ThickLineMesh();

    void DrawMe() const;
};

namespace primitives
{
    ThickLineMesh ToLineMesh(const std::vector<GLfloat>& coordinates);
}