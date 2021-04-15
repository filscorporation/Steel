#pragma once

#include "AudioTrack.h"
#include "../Resources/FileLoader.h"

class WavLoader : public FileLoader
{
public:
    static AudioTrack* LoadWav(const char* filePath, char** data);

private:
    static AudioTrack* LoadWavFileHeader(std::ifstream& file);
};
