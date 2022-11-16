#pragma once

#include <Steel/EntityComponentSystem/TypeInfo.h>

#include <yaml-cpp/node/node.h>

struct ComponentNodeWrapper
{
    YAML::Node Node;
    TypeID ComponentTypeID;
    bool IsDirty;
};
