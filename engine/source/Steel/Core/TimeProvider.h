#pragma once

class TimeProvider
{
public:
    TimeProvider() = default;
    virtual ~TimeProvider() = default;

    virtual void Init() = 0;
    virtual float GetTime() = 0;
};
