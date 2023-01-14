#pragma once

#include "Steel/Core/TimeProvider.h"

class AndroidTimeProvider : public TimeProvider
{
public:
    AndroidTimeProvider() = default;
    ~AndroidTimeProvider() override = default;

    float GetTime() override;
};
