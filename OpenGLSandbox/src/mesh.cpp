#include "mesh.h"

using namespace std;

constexpr auto siz = sizeof(GLfloat);

void PrepareInputAssembler()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * siz, (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * siz, (GLvoid*)(3 * siz));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

Mesh::Mesh(const vector<GLfloat>& vertices)
{
    glGenVertexArrays(1, &ID);
    Bind();

    // VB
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * siz, vertices.data(), GL_STATIC_DRAW);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * siz, indices.data(), GL_STATIC_DRAW);

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
