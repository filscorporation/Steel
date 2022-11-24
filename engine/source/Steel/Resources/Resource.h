#pragma once

#include "ResourceID.h"
#include "ResourceType.h"
#include "Steel/Serialization/Serializable.h"
#include "Steel/EntityComponentSystem/TypeInfo.h"

#include <string>

class Resource : public Serializable
{
public:
    ~Resource() override = default;

    ResourceID ID = NULL_RESOURCE;
    ResourceTypes::ResourceType Type = ResourceTypes::Undefined;
    std::string FullPath;
};
