#include "VertexBuffer.h"
#include "OpenGLAPI.h"

void VertexBuffer::Create(glm::vec3* vertices, glm::vec4 color, glm::vec2* textureCoords)
{
    Clear();

    Size = 9 * 4;
    Data = new float[Size];

    uint32_t offset = 0;
    for (int i = 0; i < 4; ++i)
    {
        Data[offset++] = vertices[i][0];
        Data[offset++] = vertices[i][1];
        Data[offset++] = vertices[i][2];
        Data[offset++] = color[0];
        Data[offset++] = color[1];
        Data[offset++] = color[2];
        Data[offset++] = color[3];
        Data[offset++] = textureCoords[i][0];
        Data[offset++] = textureCoords[i][1];
    }

    Attributes.reserve(3);
    Attributes.emplace_back(0, 3);
    Attributes.emplace_back(1, 4);
    Attributes.emplace_back(2, 2);

    BlockSize = 9;
    VerticesCount = 4;
}

void VertexBuffer::Create(float* data, uint32_t size, const std::vector<VertexAttribute>& attributes, uint32_t verticesCount)
{
    Data = data;
    Size = size;
    Attributes = attributes;

    BlockSize = 0;
    for (auto& Attribute : Attributes)
        BlockSize += Attribute.Size;
    VerticesCount = verticesCount;
}

void VertexBuffer::Clear()
{
    delete[] Data;
    Data = nullptr;
    Size = 0;

    Attributes.clear();
    BlockSize = 0;
}

bool VertexBuffer::IsEmpty() const
{
    return Size == 0;
}
