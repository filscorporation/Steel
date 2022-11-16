#pragma once

#include "TypeInfo.h"

struct RawComponentData
{
    RawComponentData()
    {
        Data = nullptr;
        DataTypeID = 0;
    }

    RawComponentData(void* data, TypeID typeID)
    {
        Data = data;
        DataTypeID = typeID;
    }

    void* Data;
    TypeID DataTypeID;
};
