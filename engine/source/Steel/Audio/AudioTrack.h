#pragma once

#include <cstdint>

#include "Steel/Resources/Resource.h"

class AudioTrack : public Resource
{
    DEFINE_TYPE(AudioTrack)

public:
    AudioTrack();
    ~AudioTrack() override;

    uint32_t BufferID;
    int SampleRate;
    int BitsPerSample;
    int NumberOfSamples;
    float LengthInSeconds;
    int NumberOfChannels;
};
