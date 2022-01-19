#pragma once

#include <cstdint>

using ComponentTypeID = uint64_t;

class TypeInfo
{
public:
    explicit TypeInfo(const char* className);

    const char* TypeName;
    ComponentTypeID ID;
};
