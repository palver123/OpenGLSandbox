#pragma once

#include <glad\glad.h>
#include <vector>

class LineMesh {
    GLuint _vertexBuffer;
    GLsizei _elementCount;
    bool _strip;

    void Bind() const;

public:
    GLuint ID;

    LineMesh(const std::vector<GLfloat>& vertices, bool strip);
    ~LineMesh();

    void DrawMe() const;

    static LineMesh SplitPolyline(const std::vector<GLfloat>& vertices);
};
