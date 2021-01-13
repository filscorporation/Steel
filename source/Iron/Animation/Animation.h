#pragma once

#include <vector>

// TODO: make type independent keyframes
struct Keyframe
{
    float Time;
    unsigned int ImageID;
};

struct Curve
{
    std::vector<Keyframe> Keyframes;
};

class Animation
{
public:
    const char* Name;
    bool Loop = false;
    std::vector<Curve> Curves;
};
