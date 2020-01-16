#include "lineMesh.h"

using namespace std;

namespace {
	void PrepareInputAssembler()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, LineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, LineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, LineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
}

LineMesh::LineMesh(const vector<GLfloat>& vertices):
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
    for (int i = 0; i < numTriangles / 2; ++i)
    {
		const auto base = 4 * i;
		indices.push_back(base + 0);
		indices.push_back(base + 1);
		indices.push_back(base + 2);

		indices.push_back(base + 3);
		indices.push_back(base + 2);
		indices.push_back(base + 1);
	}
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

LineMesh::~LineMesh()
{
	glDeleteVertexArrays(1, &ID);
	glDeleteBuffers(1, &vertexBuffer);
	if (indexBuffer > 0)
		glDeleteBuffers(1, &indexBuffer);
}

void LineMesh::Bind() const
{
	glBindVertexArray(ID);
}

void LineMesh::DrawMe() const {
	glDrawElements(GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, nullptr);
}

#define EXPAND(v, x, y, z, ax, ay, az) \
	v.push_back(x);	\
    v.push_back(y);	\
    v.push_back(z);	\
    v.push_back(ax);	\
    v.push_back(ay);	\
    v.push_back(az);	\
	v.push_back(1.0f); \
	v.push_back(x);	\
    v.push_back(y);	\
    v.push_back(z);	\
    v.push_back(ax);	\
    v.push_back(ay);	\
    v.push_back(az);	\
	v.push_back(-1.0f);

#define EXPAND_START(v, x, y, z, nx, ny, nz) EXPAND(v, x, y, z, nx, ny, nz)
#define EXPAND_END(v, x, y, z, px, py, pz)  EXPAND(v, x, y, z, px, py, pz)

LineMesh primitives::ToLineMesh(const std::vector<GLfloat>& coordinates)
{
	auto plLength = coordinates.size() / 3;
	if (plLength < 2 || coordinates.size() % 3 != 0)
		throw exception("Number of vertices is not OK.");

	vector<GLfloat> vertices;
	vertices.reserve((plLength - 1) * 4 * LineMesh::kVertexStride);
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

	return LineMesh{ vertices };
}
