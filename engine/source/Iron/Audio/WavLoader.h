#pragma once

#include "AudioTrack.h"

class WavLoader
{
public:
    static AudioTrack* LoadWav(const char* filePath, char** data);
};
