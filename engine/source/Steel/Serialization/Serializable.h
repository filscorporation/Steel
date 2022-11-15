#pragma once

#include "Steel/EntityComponentSystem/TypeInfo.h"

class Serializable
{
public:
    virtual ~Serializable() = default;

    virtual const TypeInfo* GetTypeInfo() = 0;
};
