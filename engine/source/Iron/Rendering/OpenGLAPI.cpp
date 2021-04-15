#include "OpenGLAPI.h"

#include <GLAD/glad.h>

uint32_t OpenGLAPI::GenerateTexture(unsigned char* imageData, uint32_t width, uint32_t height)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLint)width, (GLint)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    EndGenerateTexture();

    return textureID;
}

void OpenGLAPI::DeleteTexture(uint32_t textureID)
{
    glDeleteTextures(1, &textureID);
}

uint32_t OpenGLAPI::BeginGenerateMonochromeTexture(uint32_t width, uint32_t height)
{
    GLuint textureID;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (GLint)width, (GLint)height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    return textureID;
}

void OpenGLAPI::SetMonochromeTextureSubImage(unsigned char* imageData, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, (GLint)width, (GLint)height, GL_RED, GL_UNSIGNED_BYTE, imageData);
}

void OpenGLAPI::EndGenerateTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
