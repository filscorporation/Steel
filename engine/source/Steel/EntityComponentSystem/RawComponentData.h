#pragma once

#include "TypeInfo.h"

struct RawComponentData
{
    RawComponentData()
    {
        Data = nullptr;
        TypeID = 0;
    }

    RawComponentData(void* data, ComponentTypeID typeID)
    {
        Data = data;
        TypeID = typeID;
    }

    void* Data;
    ComponentTypeID TypeID;
};
