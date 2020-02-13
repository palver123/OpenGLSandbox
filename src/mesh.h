#pragma once

#include <glad\glad.h>
#include <vector>

class Mesh {
    GLuint vertexBuffer, indexBuffer;
    GLsizei _elementCount;

    void Bind() const;

public:
    static constexpr GLsizei kVertexStride = 5;
    GLuint ID;

    Mesh(const std::vector<GLfloat>& vertices);
    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint> indices);
    ~Mesh();

    void DrawMe() const;
};

namespace primitives
{
    Mesh Square(GLfloat halfSize = 0.5f);
    Mesh Cube();
}