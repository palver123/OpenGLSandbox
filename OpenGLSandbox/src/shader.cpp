#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace {
    string ReadFile(const GLchar* path)
    {
        string content;
        ifstream file;
        file.exceptions(ifstream::failbit | ifstream::badbit);

        try
        {
            file.open(path);

            stringstream ss;
            ss << file.rdbuf();
            file.close();

            content = ss.str();
        }
        catch (const ifstream::failure e)
        {
            cout << "Failed to read shader file '" << path << "':\n" << e.what() << endl;
        }

        return content;
    }
}

bool CompileShader(GLenum type, const GLchar * sourcePath, GLuint& shader)
{
    const auto sourceCode = ReadFile(sourcePath);
    const auto cstr = sourceCode.c_str();

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    static GLchar infoLog[512];
    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        cout << infoLog << endl;
        return false;
    }

    return true;
}

bool LinkShaders(GLuint vertexShader, GLuint fragmentShader, GLuint& program)
{
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Report errors (if any)
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        cout << infoLog << endl;
        return false;
    }

    return true;
}

Shader::Shader(const GLchar * vertexSourcePath, const GLchar * fragmentSourcePath)
{
    GLuint vertexShader;
    if (!CompileShader(GL_VERTEX_SHADER, vertexSourcePath, vertexShader))
        throw runtime_error("Failed to compile vertex shader");

    GLuint fragmentShader;
    if (!CompileShader(GL_FRAGMENT_SHADER, fragmentSourcePath, fragmentShader))
    {
        glDeleteShader(vertexShader);
        throw runtime_error("Failed to compile fragment shader");
    }

    const auto linkedSuccessfully = LinkShaders(vertexShader, fragmentShader, Program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (!linkedSuccessfully)
        throw runtime_error("Failed to link shader program");
}

void Shader::Use() const
{
    glUseProgram(Program);
}
