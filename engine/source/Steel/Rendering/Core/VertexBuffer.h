#pragma once

#include "VertexAttribute.h"

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct VertexBuffer
{
    void Create(glm::vec3 vertices[4], glm::vec4 color, glm::vec2 textureCoords[4]);
    void Create(float* data, uint32_t size, const std::vector<VertexAttribute>& attributes, uint32_t verticesCount);
    void Clear();
    bool IsEmpty() const;

    float* Data = nullptr;
    uint32_t Size = 0;
    std::vector<VertexAttribute> Attributes;
    uint32_t BlockSize;
    uint32_t VerticesCount;
};
