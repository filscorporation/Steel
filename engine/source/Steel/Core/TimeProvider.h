#pragma once

class TimeProvider
{
public:
    TimeProvider() = default;
    virtual ~TimeProvider() = default;

    virtual float GetTime() = 0;
};
