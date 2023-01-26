#pragma once

#include "Steel/Core/TimeProvider.h"

class DesktopTimeProvider : public TimeProvider
{
public:
    DesktopTimeProvider() = default;
    ~DesktopTimeProvider() override = default;

    void Init() override;
    float GetTime() override;
};
