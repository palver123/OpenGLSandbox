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



void uniforms::ModelViewProj::Locate(GLint shader) {
    lModel = glGetUniformLocation(shader, "model");
    lView = glGetUniformLocation(shader, "view");
    lProj = glGetUniformLocation(shader, "projection");
}

void uniforms::Basic::Locate(GLint shader) {
    ModelViewProj::Locate(shader);
    lBlendFactor = glGetUniformLocation(shader, "texBlendFactor");
}

void uniforms::ThickLines::Locate(GLint shader) {
    ModelViewProj::Locate(shader);
    lColor = glGetUniformLocation(shader, "matDiffColor");
    lThickness = glGetUniformLocation(shader, "thickness");
    lAspect = glGetUniformLocation(shader, "aspectRatio");
}

void uniforms::ModelViewProj::Submit(const GLfloat** data) const {
    glUniformMatrix4fv(lModel, 1, GL_FALSE, data[0]);
    glUniformMatrix4fv(lView, 1, GL_FALSE, data[1]);
    glUniformMatrix4fv(lProj, 1, GL_FALSE, data[2]);
}

void uniforms::Basic::Submit(const GLfloat** data) const {
    ModelViewProj::Submit(data);
    glUniform1f(lBlendFactor, *data[3]);
}

void uniforms::ThickLines::Submit(const GLfloat** data) const {
    ModelViewProj::Submit(data);
    const auto color = data[3];
    glUniform3f(lColor, color[0], color[1], color[2]);
    glUniform1f(lThickness, *data[4] / 600.0f);
    glUniform1f(lAspect, *data[5]);
}