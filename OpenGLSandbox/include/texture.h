#pragma once

#include <glad\glad.h>

class Texture {
    unsigned int _texUnit;
    GLint _locOfSamplerVariable;

public:
    GLuint ID;

    Texture(const char* filename, unsigned texUnit = 0);
    ~Texture();

    void Init(GLuint shaderProgram, const GLchar* nameOfVariable);
    void Bind() const;
};
