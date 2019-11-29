#pragma once

#include "glews.h"

class Shader
{
public:
    // The program ID
    GLuint Program;

    // Constructor reads and builds the shader
    Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);

    // Use the program
    void Use() const;
};