#include <fstream>
#include <cstring>

#include "WavLoader.h"
#include "../Core/Log.h"

AudioTrack* WavLoader::LoadWavFileHeader(std::ifstream& file)
{
    char buffer[4];
    if (!file.is_open())
        return nullptr;

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read RIFF");
        return nullptr;
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (header doesn't begin with RIFF)");
        return nullptr;
    }

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read size of file");
        return nullptr;
    }

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read WAVE");
        return nullptr;
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (header doesn't contain WAVE)");
        return nullptr;
    }

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read fmt/0");
        return nullptr;
    }

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read the 16");
        return nullptr;
    }

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read PCM");
        return nullptr;
    }

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read number of channels");
        return nullptr;
    }

    auto audioTrack = new AudioTrack();
    audioTrack->NumberOfChannels = ConvertToInt(buffer, 2);

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read sample rate");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->SampleRate = ConvertToInt(buffer, 4);

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read (sampleRate * bitsPerSample * channels) / 8");
        delete audioTrack;
        return nullptr;
    }

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read wave structure");
        delete audioTrack;
        return nullptr;
    }

    if (!file.read(buffer, 2))
    {
        Log::LogError("Could not read bits per sample");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->BitsPerSample = ConvertToInt(buffer, 2);

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read data chunk header");
        delete audioTrack;
        return nullptr;
    }
    if (std::strncmp(buffer, "data", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (doesn't have 'data' tag)");
        delete audioTrack;
        return nullptr;
    }

    if (!file.read(buffer, 4))
    {
        Log::LogError("Could not read data size");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->NumberOfSamples = ConvertToInt(buffer, 4);

    if (file.eof())
    {
        Log::LogError("Reached EOF on the file");
        delete audioTrack;
        return nullptr;
    }
    if (file.fail())
    {
        Log::LogError("Fail state set on the file");
        delete audioTrack;
        return nullptr;
    }

    return audioTrack;
}

AudioTrack* WavLoader::LoadWav(const char* filePath, char** data)
{
    std::ifstream in(filePath, std::ios::binary);
    if (!in.is_open())
    {
        Log::LogError("Could not open " + std::string(filePath));
        return nullptr;
    }

    auto audioTrack = LoadWavFileHeader(in);
    if (audioTrack == nullptr)
    {
        Log::LogError("Could not load wav header of " + std::string(filePath));
        return nullptr;
    }

    audioTrack->LengthInSeconds = (float)audioTrack->NumberOfSamples / (float)audioTrack->SampleRate;
    *data = new char[audioTrack->NumberOfSamples];
    in.read(*data, audioTrack->NumberOfSamples);

    return audioTrack;
}
