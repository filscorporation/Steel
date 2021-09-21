#pragma once

#include <stdint.h>

#define NULL_ENTITY 0xffffffff

using EntityID = uint32_t;

namespace EntityStates
{
    enum EntityState
    {
        IsActive =      1 << 0,
        IsActiveSelf =  1 << 1,
    };

    inline EntityState operator~(EntityState a)
    {
        return static_cast<EntityState>(~static_cast<uint32_t>(a));
    }

    inline EntityState operator|(EntityState a, EntityState b)
    {
        return static_cast<EntityState>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline EntityState operator&(EntityState a, EntityState b)
    {
        return static_cast<EntityState>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
}
