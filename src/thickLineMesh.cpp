#include "thickLineMesh.h"

using namespace std;

namespace {
    constexpr auto kVertexStrideForLineStrips = 3;
    constexpr auto kVertexStrideForLines = 6;

    struct Vector3 {
        GLfloat x, y, z;

        Vector3 Subtract(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        Vector3 Scale(const GLfloat s) const { return { x * s, y * s, z * s }; }
    };

    void Store(const Vector3& v, std::vector<GLfloat>& coords) {
        coords.push_back(v.x);
        coords.push_back(v.y);
        coords.push_back(v.z);
    }

    void Store2(const Vector3& v, std::vector<GLfloat>& coords) {
        Store(v, coords);
        Store(v, coords);
    }
}

ThickLineMesh::ThickLineMesh(const vector<GLfloat>& vertices, bool strip):
    _strip(strip)
{
    glGenVertexArrays(1, &ID);
    Bind();

    // VB
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    if (strip) {
        _elementCount = static_cast<GLsizei>(vertices.size() / kVertexStrideForLineStrips - 4);

        // IA
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexStrideForLineStrips * sizeof(GLfloat), (GLvoid*)(2 * kVertexStrideForLineStrips * sizeof(GLfloat)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexStrideForLineStrips * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, kVertexStrideForLineStrips * sizeof(GLfloat), (GLvoid*)(4 * kVertexStrideForLineStrips * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // IB
        _indexBuffer = -1;
    }
    else {
        constexpr auto kVertexStride = 6;
        _elementCount = static_cast<GLsizei>(3 * vertices.size() / kVertexStride / 2);

        // IA
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexStride * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

        // IB
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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glLineWidth(1.5f);
    if (_strip)
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _elementCount);
    else 
        glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
}

bool ThickLineMesh::IsStrip() const { return _strip; }

#define EXPAND(v, x, y, z, ax, ay, az) \
    v.push_back(x);     \
    v.push_back(y);     \
    v.push_back(z);     \
    v.push_back(ax);    \
    v.push_back(ay);    \
    v.push_back(az);    \
    v.push_back(x);     \
    v.push_back(y);     \
    v.push_back(z);     \
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
    vertices.reserve((plLength - 1) * 4 * kVertexStrideForLines);
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

    vector<GLfloat> vertices;
    vertices.reserve((plLength + 2) * 2 * kVertexStrideForLineStrips);
    auto curr = Vector3{ coordinates[0], coordinates[1], coordinates[2] };
    auto next = Vector3{ coordinates[3], coordinates[4], coordinates[5] };
    auto prev = curr.Scale(2).Subtract(next);
    Store2(prev, vertices);

    for (size_t i = 1; i < plLength; ++i)
    {
        Store2(curr, vertices);
        prev = curr;
        curr = Vector3{ coordinates[3 * i], coordinates[3 * i + 1], coordinates[3 * i + 2] };
    }

    Store2(curr, vertices);
    next = curr.Scale(2).Subtract(prev);
    Store2(next, vertices);

    return ThickLineMesh(vertices, true);
}
