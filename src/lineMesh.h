#pragma once

#include <glad\glad.h>
#include <vector>

class LineMesh {
    GLuint vertexBuffer;
    GLsizei numIndices;
    bool _strip;

    void Bind() const;

public:
    static constexpr GLsizei kVertexStride = 3;
    GLuint ID;

    LineMesh(const std::vector<GLfloat>& vertices, bool strip);
    ~LineMesh();

    void DrawMe() const;

    static LineMesh SplitPolyline(const std::vector<GLfloat>& vertices);
};
