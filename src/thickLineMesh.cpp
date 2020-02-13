#include "thickLineMesh.h"

using namespace std;

namespace {
    void PrepareInputAssembler()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ThickLineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ThickLineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
}

ThickLineMesh::ThickLineMesh(const vector<GLfloat>& vertices):
    numTriangles(static_cast<GLsizei>(vertices.size() / kVertexStride / 2))
{
    glGenVertexArrays(1, &ID);
    Bind();

    // VB
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // IA
    PrepareInputAssembler();

    std::vector<GLuint> indices;
    indices.reserve(numTriangles * 3);
    for (int quadIdx = 0; quadIdx < numTriangles / 2; ++quadIdx)
    {
        const auto baseVIdx = 4 * quadIdx;
        indices.push_back(baseVIdx + 0);
        indices.push_back(baseVIdx + 1);
        indices.push_back(baseVIdx + 2);

        indices.push_back(baseVIdx + 3);
        indices.push_back(baseVIdx + 2);
        indices.push_back(baseVIdx + 1);
    }
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

ThickLineMesh::~ThickLineMesh()
{
    glDeleteVertexArrays(1, &ID);
    glDeleteBuffers(1, &vertexBuffer);
    if (indexBuffer > 0)
        glDeleteBuffers(1, &indexBuffer);
}

void ThickLineMesh::Bind() const
{
    glBindVertexArray(ID);
}

void ThickLineMesh::DrawMe() const {
    Bind();
    glDrawElements(GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, nullptr);
}

#define EXPAND(v, x, y, z, ax, ay, az) \
    v.push_back(x);    \
    v.push_back(y);    \
    v.push_back(z);    \
    v.push_back(ax);    \
    v.push_back(ay);    \
    v.push_back(az);    \
    v.push_back(x);    \
    v.push_back(y);    \
    v.push_back(z);    \
    v.push_back(ax);    \
    v.push_back(ay);    \
    v.push_back(az);    \

#define EXPAND_START(v, x, y, z, nx, ny, nz) EXPAND(v, x, y, z, nx, ny, nz)
#define EXPAND_END(v, x, y, z, px, py, pz)  EXPAND(v, x, y, z, px, py, pz)

ThickLineMesh primitives::ToLineMesh(const std::vector<GLfloat>& coordinates)
{
    auto plLength = coordinates.size() / 3;
    if (plLength < 2 || coordinates.size() % 3 != 0)
        return ThickLineMesh{ {} };

    vector<GLfloat> vertices;
    vertices.reserve((plLength - 1) * 4 * ThickLineMesh::kVertexStride);
    auto prevX = coordinates[0];
    auto prevY = coordinates[1];
    auto prevZ = coordinates[2];

    EXPAND_START(vertices, prevX, prevY, prevZ, coordinates[3], coordinates[4], coordinates[5]);
    for (size_t i = 1; i < plLength - 1; ++i)
    {
        const auto x = coordinates[3 * i];
        const auto y = coordinates[3 * i + 1];
        const auto z = coordinates[3 * i + 2];
        const auto nextX = coordinates[3 * i + 3];
        const auto nextY = coordinates[3 * i + 4];
        const auto nextZ = coordinates[3 * i + 5];

        EXPAND_END(vertices, x, y, z, prevX, prevY, prevZ);
        EXPAND_START(vertices, x, y, z, nextX, nextY, nextZ);

        prevX = x;
        prevY = y;
        prevZ = z;
    }

    const auto lastIdx = coordinates.size() - 3;
    EXPAND_END(vertices, coordinates[lastIdx], coordinates[lastIdx + 1], coordinates[lastIdx + 2], prevX, prevY, prevZ);

    return ThickLineMesh{ vertices };
}
