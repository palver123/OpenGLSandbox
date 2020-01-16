#include "lineMesh.h"

using namespace std;

namespace {
	void PrepareInputAssembler()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, LineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, LineMesh::kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
}

LineMesh::LineMesh(const vector<GLfloat>& vertices):
	numTriangles(static_cast<GLsizei>(vertices.size() / kVertexStride))
{
	glGenVertexArrays(1, &ID);
	Bind();

	// VB
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	// IA
	PrepareInputAssembler();

	glBindVertexArray(0);
}

LineMesh::~LineMesh()
{
	glDeleteVertexArrays(1, &ID);
	glDeleteBuffers(1, &vertexBuffer);
}

void LineMesh::Bind() const
{
	glBindVertexArray(ID);
}

void LineMesh::DrawMe() const {
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTriangles);
}

LineMesh primitives::ToLineMesh(const std::vector<GLfloat>& coordinates)
{
	vector<GLfloat> vertices;
	vertices.reserve(coordinates.size() / 3 * LineMesh::kVertexStride * 2);
	GLfloat prevX, prevY, prevZ;
    for (size_t i = 0; i < coordinates.size(); i += 3)
    {
		const auto x = coordinates[i];
		const auto y = coordinates[i + 1];
		const auto z = coordinates[i + 2];

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(prevX);
		vertices.push_back(prevY);
		vertices.push_back(prevZ);
		vertices.push_back(-1.0f);

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(prevX);
		vertices.push_back(prevY);
		vertices.push_back(prevZ);
		vertices.push_back(1.0f);

		prevX = x;
		prevY = y;
		prevZ = z;
	}

	return LineMesh{ vertices };
}
