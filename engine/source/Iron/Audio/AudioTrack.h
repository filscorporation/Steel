#pragma once

#include <cstdint>

#include "../Resources/ResourceID.h"

struct AudioTrack
{
public:
    ResourceID ID;
    const char* Path;
    uint32_t BufferID;
    int SampleRate;
    int BitsPerSample;
    int NumberOfSamples;
    float LengthInSeconds;
    int NumberOfChannels;
};
