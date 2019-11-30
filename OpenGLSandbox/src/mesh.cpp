#include "mesh.h"

Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint> indices)
{
    constexpr auto siz = sizeof(GLfloat);
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);

    // VB
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * siz, vertices.data(), GL_STATIC_DRAW);

    // IB
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * siz, indices.data(), GL_STATIC_DRAW);

    // IA
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * siz, (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * siz, (GLvoid*)(3 * siz));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &ID);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Mesh::Use() const
{
    glBindVertexArray(ID);
}
