#pragma once

#include <stdint.h>

#include "Steel/Common/FlagType.h"

#define NULL_ENTITY 0xffffffff

using EntityID = uint32_t;

namespace EntityStates
{
    enum EntityState
    {
        IsActive =      1 << 0,
        IsActiveSelf =  1 << 1,
    };

    FLAG_TYPE(EntityState)
}
