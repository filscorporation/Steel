#pragma once

#include "Steel/Common/FlagType.h"

namespace ClearFlags
{
    enum ClearFlag
    {
        Color   = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2,

        All = (uint32_t)-1,
    };

    FLAG_TYPE(ClearFlag)
}

namespace StencilOperations
{
    enum StencilOperation
    {
        Keep,
        Zero,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert,
    };
}

namespace ComparisonFunctions
{
    enum ComparisonFunction
    {
        Never,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,
        Always,
    };
}
