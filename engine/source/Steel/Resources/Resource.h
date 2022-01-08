#pragma once

#include "ResourceID.h"
#include "ResourceType.h"

#include <string>

class Resource
{
public:
    virtual ~Resource() = default;

    ResourceID ID = NULL_RESOURCE;
    ResourceTypes::ResourceType Type = ResourceTypes::Undefined;
    std::string Path;
};
