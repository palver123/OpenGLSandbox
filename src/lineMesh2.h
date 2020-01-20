#pragma once

#include <glad\glad.h>
#include <vector>

class LineMesh2 {
    GLuint vertexBuffer;
    GLsizei numIndices;
    bool _strip;

    void Bind() const;

public:
    static constexpr GLsizei kVertexStride = 3;
    GLuint ID;

    LineMesh2(const std::vector<GLfloat>& vertices, bool strip);
    ~LineMesh2();

    void DrawMe() const;

    static LineMesh2 SplitPolyline(const std::vector<GLfloat>& vertices);
};
