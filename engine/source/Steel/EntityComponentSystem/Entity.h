#pragma once

#include "Steel/Common/FlagType.h"

#include <cstdint>

#define NULL_ENTITY 0xffffffff

using EntityID = uint32_t;

namespace EntityStates
{
    enum EntityState
    {
        // Entity is active in hierarchy
        IsActive =      1 << 0,
        // Entity is active by itself
        IsActiveSelf =  1 << 1,
    };

    FLAG_TYPE(EntityState)
}

namespace Entity
{
    EntityID EntityIDGetID(EntityID entityID);

    EntityID EntityIDGetVersion(EntityID entityID);

    EntityID EntityIDCombine(EntityID id, EntityID version);

    EntityID EntityIDIncrementVersion(EntityID id);
}
