#pragma once

#include <Steel/EntityComponentSystem/TypeInfo.h>

#include <yaml-cpp/node/node.h>

struct TypeNodeWrapper
{
    YAML::Node Node;
    TypeID DataTypeID;
    bool IsDirty;
};
