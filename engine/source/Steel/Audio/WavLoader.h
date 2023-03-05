#pragma once

#include "AudioTrack.h"
#include "Steel/Resources/FileLoader.h"
#include "Steel/Resources/FileDataReader.h"

class WavLoader : public FileLoader
{
public:
    static AudioTrack* LoadWav(FileDataReader& reader);

private:
    static AudioTrack* LoadWavFileHeader(FileDataReader& reader);
};
