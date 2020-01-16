#include "mesh.h"

using namespace std;

namespace {
	void PrepareInputAssembler()
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Mesh::kVertexStride * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Mesh::kVertexStride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
}

Mesh::Mesh(const vector<GLfloat>& vertices):
	numTriangles(vertices.size() / kVertexStride * 2)
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

Mesh::Mesh(const vector<GLfloat>& vertices, const vector<GLuint> indices) : Mesh{vertices}
{
    Bind();

    // IB
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &ID);
    glDeleteBuffers(1, &vertexBuffer);
    if (indexBuffer > 0)
        glDeleteBuffers(1, &indexBuffer);
}

void Mesh::Bind() const
{
    glBindVertexArray(ID);
}

void Mesh::DrawMe() const {
	glDrawArrays(GL_TRIANGLES, 0, numTriangles);
}

Mesh primitives::Square(GLfloat halfSize)
{
    const GLfloat tex = 1.0;
    vector<GLfloat> vertices = {
         halfSize,  halfSize, 0, tex, tex,  // Top Right
         halfSize, -halfSize, 0, tex, 0,    // Bottom Right
        -halfSize, -halfSize, 0, 0, 0,      // Bottom Left
        -halfSize,  halfSize, 0, 0, tex     // Top Left
    };
    vector<GLuint> indices = { // Note that we start from 0!
        0, 1, 3, // First Triangle
        3, 2, 1 // Second Triangle
    };

    return Mesh{ vertices, indices };
}

Mesh primitives::Cube()
{
    const GLfloat half = 0.5f;
    const GLfloat tex = 1.0;
    vector<GLfloat> vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    return Mesh{ vertices };
}
