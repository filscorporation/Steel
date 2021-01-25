#pragma once

struct AudioTrack
{
public:
    unsigned int ID;
    const char* Path;
    unsigned int BufferID;
    int SampleRate;
    int BitsPerSample;
    int NumberOfSamples;
    float LengthInSeconds;
    int NumberOfChannels;
};
