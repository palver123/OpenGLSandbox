#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* filename, unsigned texUnit):
    _texUnit(texUnit > 15u ? 15u : texUnit)
{
    glGenTextures(1, &ID);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    const auto image = stbi_load(filename, &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::Init(GLuint shaderProgram, const GLchar* nameOfVariable)
{
    _locOfSamplerVariable = glGetUniformLocation(shaderProgram, nameOfVariable);
}

void Texture::Bind() const
{
    glActiveTexture(GL_TEXTURE0 + _texUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glUniform1i(_locOfSamplerVariable, _texUnit);
}
