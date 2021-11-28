#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct VertexBuffer
{
    void Create(glm::vec3 vertices[4], glm::vec4 color, glm::vec2 textureCoords[4]);
    void Clear();
    bool IsEmpty();

    void Bind();
    void Unbind();

    float* Data = nullptr;
    uint32_t Size = 0;
    std::vector<uint32_t> AttributeBlocks;
    uint32_t BlockSize;
    uint32_t VerticesCount;
};
