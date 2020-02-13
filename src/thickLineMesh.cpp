#include "thickLineMesh.h"

using namespace std;

namespace {
    static constexpr GLsizei kVertexStride = 6;

    void PrepareInputAssembler()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexStride * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    struct Vector3 {
        GLfloat x, y, z;

        Vector3 Add(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
        Vector3 Subtract(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        Vector3 Scale(const GLfloat s) const { return { x * s, y * s, z * s }; }

        Vector3 RotatedCCW90() const { return { -y, x, z }; }
    };

    void Store(const Vector3& v, GLfloat* coords, int i) {
        coords[i] = v.x;
        coords[i + 1] = v.y;
        coords[i + 2] = v.z;
    }

    void Store2(const Vector3& v, GLfloat* coords, int i) {
        Store(v, coords, i);
        Store(v, coords, i + kVertexStride);
    }

    void Store2Normals(const Vector3& v, GLfloat* coords, int i) {
        Store(v, coords, i);
        Store(v.Scale(-1), coords, i + kVertexStride);
    }

    Vector3 CalculateNormal(const Vector3& prev, const Vector3& next) {
        return next.Subtract(prev).Scale(0.5f).RotatedCCW90();
    }
}

ThickLineMesh::ThickLineMesh(const vector<GLfloat>& vertices, bool strip):
    _elementCount(static_cast<GLsizei>(vertices.size() / kVertexStride * (strip ? 1 : (3.0 / 2)))),
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

    if (strip) {
        _indexBuffer = -1;
    } else {
        std::vector<GLuint> indices;
        indices.reserve(_elementCount);
        for (int quadIdx = 0; quadIdx < _elementCount / 6; ++quadIdx)
        {
            const auto baseVIdx = 4 * quadIdx;
            indices.push_back(baseVIdx + 0);
            indices.push_back(baseVIdx + 1);
            indices.push_back(baseVIdx + 2);

            indices.push_back(baseVIdx + 3);
            indices.push_back(baseVIdx + 2);
            indices.push_back(baseVIdx + 1);
        }
        glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

ThickLineMesh::~ThickLineMesh()
{
    glDeleteVertexArrays(1, &ID);
    glDeleteBuffers(1, &_vertexBuffer);
    if (_indexBuffer > 0)
        glDeleteBuffers(1, &_indexBuffer);
}

void ThickLineMesh::Bind() const
{
    glBindVertexArray(ID);
}

void ThickLineMesh::DrawMe() const {
    Bind();
    if (_strip)
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _elementCount);
    else 
        glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
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
        return ThickLineMesh{ {}, false };

    vector<GLfloat> vertices;
    vertices.reserve((plLength - 1) * 4 * kVertexStride);
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

    return ThickLineMesh{ vertices, false };
}

ThickLineMesh primitives::ToLineStripMesh(const std::vector<GLfloat>& coordinates) {
    auto plLength = coordinates.size() / 3;
    if (plLength < 2 || coordinates.size() % 3 != 0)
        return ThickLineMesh{ {}, true };

    vector<GLfloat> vertices(plLength * 2 * kVertexStride);
    Vector3 prev;
    auto curr = Vector3{ coordinates[0], coordinates[1], coordinates[2] };
    auto next = Vector3{ coordinates[3], coordinates[4], coordinates[5] };
    Store2(curr, vertices.data(), 0);
    Store2Normals(CalculateNormal(curr.Scale(2).Subtract(next), next), vertices.data(), 3);

    for (size_t i = 1; i < plLength - 1; ++i)
    {
        prev = curr;
        curr = next;
        next = Vector3{ coordinates[3 * i + 3], coordinates[3 * i + 4], coordinates[3 * i + 5] };

        Store2(curr, vertices.data(), i * 2 * kVertexStride);
        Store2Normals(CalculateNormal(prev, next), vertices.data(), i * 2 * kVertexStride + 3);
    }

    Store2(next, vertices.data(), (plLength - 1) * 2 * kVertexStride);
    Store2Normals(CalculateNormal(prev, curr.Scale(2).Subtract(prev)), vertices.data(), (plLength - 1) * 2 * kVertexStride + 3);
    return ThickLineMesh(vertices, true);
}
