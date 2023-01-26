#include "OpenGLAPI.h"

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
#include <GLAD/glad.h>
#endif
#if defined PLATFORM_ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif

#undef Always

int OpenGLAPI::Init()
{

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    return gladLoadGL();
#endif
#if defined PLATFORM_ANDROID
    return 1; // Android OpenGL ES setup is in native code
#endif
}

void OpenGLAPI::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void OpenGLAPI::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

void OpenGLAPI::EnableStencilTest()
{
    glEnable(GL_STENCIL_TEST);
}

void OpenGLAPI::DisableStencilTest()
{
    glDisable(GL_STENCIL_TEST);
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

void OpenGLAPI::Flush()
{
    glFlush();
}

void OpenGLAPI::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGLAPI::ClearAll()
{
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLAPI::ClearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLAPI::ClearDepth()
{
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLAPI::ClearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

int GLPolygonMode(OpenGLAPI::PolygonMode mode)
{
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    switch (mode)
    {
        case OpenGLAPI::Fill:
            return GL_FILL;
        case OpenGLAPI::Line:
            return GL_LINE;
    }

    return -1;
#endif
#if defined PLATFORM_ANDROID
    return 0; // Not supported
#endif
}

void OpenGLAPI::SetPolygonMode(PolygonMode mode)
{
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    glPolygonMode(GL_FRONT_AND_BACK, GLPolygonMode(mode));
#endif
#if defined PLATFORM_ANDROID
    // Not supported
#endif
}

void OpenGLAPI::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

int GLComparisonFunc(ComparisonFunctions::ComparisonFunction func)
{
    switch (func)
    {
        case ComparisonFunctions::Never:
            return GL_NEVER;
        case ComparisonFunctions::Less:
            return GL_LESS;
        case ComparisonFunctions::LessEqual:
            return GL_LEQUAL;
        case ComparisonFunctions::Greater:
            return GL_GREATER;
        case ComparisonFunctions::GreaterEqual:
            return GL_GEQUAL;
        case ComparisonFunctions::Equal:
            return GL_EQUAL;
        case ComparisonFunctions::NotEqual:
            return GL_NOTEQUAL;
        case ComparisonFunctions::Always:
            return GL_ALWAYS;
    }

    return -1;
}

int GLStencilOperation(StencilOperations::StencilOperation op)
{
    switch (op)
    {
        case StencilOperations::Keep:
            return GL_KEEP;
        case StencilOperations::Zero:
            return GL_ZERO;
        case StencilOperations::Replace:
            return GL_REPLACE;
        case StencilOperations::Increment:
            return GL_INCR;
        case StencilOperations::IncrementWrap:
            return GL_INCR_WRAP;
        case StencilOperations::Decrement:
            return GL_DECR;
        case StencilOperations::DecrementWrap:
            return GL_DECR_WRAP;
        case StencilOperations::Invert:
            return GL_INVERT;
    }

    return -1;
}

void OpenGLAPI::SetDepthMask(bool mask)
{
    glDepthMask(mask);
}

void OpenGLAPI::SetDepthFunc(ComparisonFunctions::ComparisonFunction func)
{
    glDepthFunc(GLComparisonFunc(func));
}

void OpenGLAPI::SetStencilFunc(ComparisonFunctions::ComparisonFunction func, short ref, short mask)
{
    glStencilFunc(GLComparisonFunc(func), ref, mask);
}

void OpenGLAPI::SetStencilMask(short mask)
{
    glStencilMask(mask);
}

void OpenGLAPI::SetStencilOperation(StencilOperations::StencilOperation fail, StencilOperations::StencilOperation zfail, StencilOperations::StencilOperation zpass)
{
    glStencilOp(GLStencilOperation(fail), GLStencilOperation(zfail), GLStencilOperation(zpass));
}

int OpenGLAPI::GetUniformLocation(uint32_t shaderProgram, const char* name)
{
    return glGetUniformLocation(shaderProgram, name);
}

void OpenGLAPI::SetUniformFloat(int uniform, float value)
{
    glUniform1f(uniform, value);
}

void OpenGLAPI::SetUniformInt(int uniform, int value)
{
    glUniform1i(uniform, value);
}

void OpenGLAPI::SetUniformUInt(int uniform, uint32_t value)
{
    glUniform1ui(uniform, value);
}

void OpenGLAPI::SetUniformIntegers(int uniform, int valuesCount, const int* values)
{
    glUniform1iv(uniform, valuesCount, values);
}

void OpenGLAPI::SetUniformMat4F(int uniform, const float* values)
{
    glUniformMatrix4fv(uniform, 1, GL_FALSE, values);
}

void OpenGLAPI::SetUniformVec4F(int uniform, const float* values)
{
    glUniform4fv(uniform, 1, values);
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

void OpenGLAPI::SetIndexBufferSubData(uint32_t size, void* data)
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * size, data);
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

uint32_t OpenGLAPI::GenerateColorAttachmentTexture(uint32_t width, uint32_t height)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLint)width, (GLint)height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    EndGenerateTexture();

    return textureID;
}

uint32_t OpenGLAPI::GenerateDSAttachmentTexture(uint32_t width, uint32_t height)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (GLint)width, (GLint)height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    EndGenerateTexture();

    return textureID;
}

void OpenGLAPI::DeleteTexture(uint32_t textureID)
{
    glDeleteTextures(1, &textureID);
}

uint32_t OpenGLAPI::BeginGenerateTexture(uint32_t width, uint32_t height)
{
    GLuint textureID;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLint)width, (GLint)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    return textureID;
}

void OpenGLAPI::SetTextureSubImage(unsigned char* imageData, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, (GLint)width, (GLint)height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

void OpenGLAPI::EndGenerateTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLAPI::BindTexture(uint32_t textureID, uint32_t textureSlot)
{
    GLenum activeTexture = GL_TEXTURE0 + textureSlot;
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void OpenGLAPI::UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t OpenGLAPI::CreateFramebuffer()
{
    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
}

void OpenGLAPI::DeleteFramebuffer(uint32_t fbID)
{
    glDeleteFramebuffers(1, &fbID);
}

bool OpenGLAPI::FramebufferComplete()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void OpenGLAPI::BindFramebuffer(uint32_t fbID)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbID);
}

void OpenGLAPI::SetFramebufferColorAttachment(uint32_t textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    UnbindTexture();
}

void OpenGLAPI::SetFramebufferDSAttachment(uint32_t textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
    UnbindTexture();
}
