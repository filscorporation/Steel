#include "VertexBuffer.h"

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

    AttributeBlocks = { 3, 4, 2 };
    BlockSize = 9;
    VerticesCount = 4;
}

void VertexBuffer::Clear()
{
    delete[] Data;
    Data = nullptr;
    Size = 0;

    AttributeBlocks.clear();
    BlockSize = 0;
}

bool VertexBuffer::IsEmpty()
{
    return Size == 0;
}

void VertexBuffer::Bind()
{

}

void VertexBuffer::Unbind()
{

}
