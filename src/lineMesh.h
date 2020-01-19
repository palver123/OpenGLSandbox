#pragma once

#include <glad\glad.h>
#include <vector>

class LineMesh {
    GLuint vertexBuffer, indexBuffer;
    GLsizei numTriangles;

    void Bind() const;

public:
    static constexpr GLsizei kVertexStride = 6;
    GLuint ID;

    LineMesh(const std::vector<GLfloat>& vertices);
    ~LineMesh();

    void DrawMe() const;
};

namespace primitives
{
    LineMesh ToLineMesh(const std::vector<GLfloat>& coordinates);
}