#pragma once

#include <cstdint>

struct VertexAttribute
{
    VertexAttribute(uint32_t id, int size) { AttributeID = id; Size = size; }

    uint32_t AttributeID = 0;
    int Size = 0;
};
