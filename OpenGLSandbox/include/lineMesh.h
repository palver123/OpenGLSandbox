#pragma once

#include <glad\glad.h>
#include <vector>

class LineMesh {
	GLuint vertexBuffer, indexBuffer;
	GLsizei numTriangles;

public:
	static constexpr GLsizei kVertexStride = 7;
	GLuint ID;

	LineMesh(const std::vector<GLfloat>& vertices);
	~LineMesh();

	void Bind() const;
	void DrawMe() const;
};

namespace primitives
{
	LineMesh ToLineMesh(const std::vector<GLfloat>& coordinates);
}