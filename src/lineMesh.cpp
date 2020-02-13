#include "lineMesh.h"

using namespace std;

namespace {
    static constexpr GLsizei kVertexStride = 3;

    void PrepareInputAssembler()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexStride * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
}

LineMesh::LineMesh(const vector<GLfloat>& vertices, bool strip) :
    _elementCount(static_cast<GLsizei>(vertices.size() / kVertexStride)),
    _strip(strip)
{
    glGenVertexArrays(1, &ID);
    Bind();

    // VB
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // IA
    PrepareInputAssembler();

    glBindVertexArray(0);
}

LineMesh::~LineMesh()
{
    glDeleteVertexArrays(1, &ID);
    glDeleteBuffers(1, &_vertexBuffer);
}

void LineMesh::Bind() const
{
    glBindVertexArray(ID);
}

void LineMesh::DrawMe() const {
    Bind();
    glDrawArrays(_strip ? GL_LINE_STRIP : GL_LINES, 0, _elementCount);
}

LineMesh LineMesh::SplitPolyline(const std::vector<GLfloat>& coordinates) {
    const auto numPoints = coordinates.size() / 3;
    if (coordinates.empty() || numPoints < 2)
        return LineMesh({}, false);

    std::vector<GLfloat> vertices;
    vertices.reserve((numPoints - 1) * 6);
    vertices.push_back(coordinates[0]);
    vertices.push_back(coordinates[1]);
    vertices.push_back(coordinates[2]);

    for (size_t i = 1; i < numPoints - 1; i++)
    {
        vertices.push_back(coordinates[i * 3]);
        vertices.push_back(coordinates[i * 3 + 1]);
        vertices.push_back(coordinates[i * 3 + 2]);
        vertices.push_back(coordinates[i * 3]);
        vertices.push_back(coordinates[i * 3 + 1]);
        vertices.push_back(coordinates[i * 3 + 2]);
    }

    const auto last = (numPoints - 1) * 3;
    vertices.push_back(coordinates[last]);
    vertices.push_back(coordinates[last + 1]);
    vertices.push_back(coordinates[last + 2]);

    return LineMesh{ vertices, false };
}
