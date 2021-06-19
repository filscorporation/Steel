#include "OpenGLAPI.h"

#include <GLAD/glad.h>

int OpenGLAPI::Init()
{
    return gladLoadGL();
}

void OpenGLAPI::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void OpenGLAPI::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

void OpenGLAPI::EnableBlend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLAPI::DisableBlend()
{
    glDisable(GL_BLEND);
}

void OpenGLAPI::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGLAPI::ClearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLAPI::ClearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLAPI::ClearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

int GLPolygonMode(OpenGLAPI::PolygonMode mode)
{
    switch (mode)
    {
        case OpenGLAPI::Fill:
            return GL_FILL;
        case OpenGLAPI::Line:
            return GL_LINE;
    }

    return -1;
}

void OpenGLAPI::SetPolygonMode(PolygonMode mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, GLPolygonMode(mode));
}

int OpenGLAPI::GetUniformLocation(uint32_t shaderProgram, const char* name)
{
    return glGetUniformLocation(shaderProgram, name);
}

void OpenGLAPI::SetUniformInteger(int uniform, int value)
{
    glUniform1i(uniform, value);
}

void OpenGLAPI::SetUniformIntegers(int uniform, int valuesCount, const int* values)
{
    glUniform1iv(uniform, valuesCount, values);
}

void OpenGLAPI::SetUniformMatrix4Float(int uniform, const float* values)
{
    glUniformMatrix4fv(uniform, 1, GL_FALSE, values);
}

int GLBufferType(OpenGLAPI::BufferType type)
{
    switch (type)
    {
        case OpenGLAPI::Stream:
            return GL_STREAM_DRAW;
        case OpenGLAPI::Static:
            return GL_STATIC_DRAW;
        case OpenGLAPI::Dynamic:
            return GL_DYNAMIC_DRAW;
    }

    return -1;
}

uint32_t OpenGLAPI::GenerateVertexArray()
{
    uint32_t id = 0;
    glGenVertexArrays(1, &id);

    return id;
}

void OpenGLAPI::BindVertexArray(uint32_t arrayID)
{
    glBindVertexArray(arrayID);
}

void OpenGLAPI::UnbindVertexArray()
{
    glBindVertexArray(0);
}

void OpenGLAPI::DeleteVertexArray(uint32_t arrayID)
{
    glDeleteVertexArrays(1, &arrayID);
}

uint32_t OpenGLAPI::GenerateVertexBuffer(int size, void* data, BufferType type)
{
    uint32_t id = 0;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size, data, GLBufferType(type));

    return id;
}

uint32_t OpenGLAPI::GenerateIndexBuffer(int size, void* data, BufferType type)
{
    uint32_t id = 0;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * size, data, GLBufferType(type));

    return id;
}

void OpenGLAPI::SetVertexBufferSubData(uint32_t size, void* data)
{
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * size, data);
}

void OpenGLAPI::EnableVertexFloatAttribute(int attributeID, int size, int step, int offset)
{
    glVertexAttribPointer(attributeID, size, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * step, (GLvoid*)(sizeof(GLfloat) * offset));
    glEnableVertexAttribArray(attributeID);
}

void OpenGLAPI::BindVertexBuffer(uint32_t bufferID)
{
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void OpenGLAPI::BindIndexBuffer(uint32_t bufferID)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void OpenGLAPI::UnbindVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLAPI::UnbindIndexBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLAPI::DeleteBuffer(uint32_t bufferID)
{
    glDeleteBuffers(1, &bufferID);
}

void OpenGLAPI::DrawTriangles(int count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

uint32_t OpenGLAPI::GenerateTexture(unsigned char* imageData, uint32_t width, uint32_t height, TextureFiltering filtering)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int glFiltering = filtering == Nearest ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering);
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

void OpenGLAPI::BindTexture(uint32_t textureID, int textureSlot)
{
    GLenum activeTexture = GL_TEXTURE0 + textureSlot;
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void OpenGLAPI::UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
