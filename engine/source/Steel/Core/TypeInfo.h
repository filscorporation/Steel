#pragma once

#include <cstdint>

class TypeInfo
{
public:
    explicit TypeInfo(const char* className);

    const char* TypeName;
    uint64_t ID;
};
