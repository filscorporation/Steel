#pragma once

#include <cstdint>

struct IndexBuffer
{
    void Create(uint32_t* data, uint32_t size);
    void Clear();
    bool IsEmpty() const;

    uint32_t* Data = nullptr;
    uint32_t Size = 0;
};
