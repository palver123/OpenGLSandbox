#pragma once

#include <glad\glad.h>

bool CompileShader(GLenum type, const GLchar * sourcePath, GLuint& shader);
bool LinkShaders(GLuint vertexShader, GLuint fragmentShader, GLuint& program);

template<typename Uniforms>
class Shader
{
    Uniforms parameters;

public:
    // The program ID
    GLuint Program;

    // Constructor reads and builds the shader
    Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
    {
        GLuint vertexShader;
        if (!CompileShader(GL_VERTEX_SHADER, vertexSourcePath, vertexShader))
            throw std::runtime_error("Failed to compile vertex shader");

        GLuint fragmentShader;
        if (!CompileShader(GL_FRAGMENT_SHADER, fragmentSourcePath, fragmentShader))
        {
            glDeleteShader(vertexShader);
            throw std::runtime_error("Failed to compile fragment shader");
        }

        const auto linkedSuccessfully = LinkShaders(vertexShader, fragmentShader, Program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (!linkedSuccessfully)
            throw std::runtime_error("Failed to link shader program");

        parameters.Locate(Program);
    }

    // Use the program
    void Use(const GLfloat** params) const {
        glUseProgram(Program);
        parameters.Submit(params);
    }
};

namespace uniforms {
    struct ModelViewProj {
        void Locate(GLint shader);
        void Submit(const GLfloat** data) const;

    protected:
        GLint lModel;
        GLint lView;
        GLint lProj;
    };

    struct Basic: ModelViewProj {
        void Locate(GLint shader);
        void Submit(const GLfloat** data) const;

    private:
        GLint lBlendFactor;
    };
}