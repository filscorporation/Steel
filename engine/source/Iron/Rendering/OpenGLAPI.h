#pragma once

#include <cstdint>

namespace OpenGLAPI
{
    enum TextureFiltering
    {
        Nearest,
        Linear,
    };

    enum BufferType
    {
        Stream,
        Static,
        Dynamic,
    };

    enum PolygonMode
    {
        Fill,
        Line,
    };

    int Init();

    void EnableDepthTest();
    void DisableDepthTest();
    void EnableBlend();
    void DisableBlend();
    void SetClearColor(float r, float g, float b, float a);
    void ClearColor();
    void ClearDepth();
    void ClearStencil();
    void SetPolygonMode(PolygonMode mode);

    int GetUniformLocation(uint32_t shaderProgram, const char* name);
    void SetUniformFloat(int uniform, float value);
    void SetUniformInt(int uniform, int value);
    void SetUniformUInt(int uniform, uint32_t value);
    void SetUniformIntegers(int uniform, int valuesCount, const int* values);
    void SetUniformMat4F(int uniform, const float* values);

    uint32_t GenerateVertexArray();
    void BindVertexArray(uint32_t arrayID);
    void UnbindVertexArray();
    void DeleteVertexArray(uint32_t arrayID);

    uint32_t GenerateVertexBuffer(int size, void* data, BufferType type);
    uint32_t GenerateIndexBuffer(int size, void* data, BufferType type);
    void SetVertexBufferSubData(uint32_t size, void* data);
    void EnableVertexFloatAttribute(int attributeID, int size, int step, int offset);
    void BindVertexBuffer(uint32_t bufferID);
    void BindIndexBuffer(uint32_t bufferID);
    void UnbindVertexBuffer();
    void UnbindIndexBuffer();
    void DeleteBuffer(uint32_t bufferID);

    void DrawTriangles(int count);

    uint32_t GenerateTexture(unsigned char* imageData, uint32_t width, uint32_t height, TextureFiltering filtering);
    uint32_t BeginGenerateTexture(uint32_t width, uint32_t height);
    void SetTextureSubImage(unsigned char* imageData, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void EndGenerateTexture();
    void DeleteTexture(uint32_t textureID);
    void BindTexture(uint32_t textureID, uint32_t textureSlot);
    void UnbindTexture();
}
