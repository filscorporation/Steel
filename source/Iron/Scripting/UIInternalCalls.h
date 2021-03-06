#pragma once

#include <cstdint>

namespace UIInternalCalls
{
    // UI image
    uint64_t UIImage_GetSprite(int64_t entityID);
    void UIImage_SetSprite(int64_t entityID, int64_t spriteID);
};
