#pragma once

#include "../Core/ResourcesManager.h"

class WavLoader
{
public:
    static AudioTrack* LoadWav(const char* filePath, char** data);
};
