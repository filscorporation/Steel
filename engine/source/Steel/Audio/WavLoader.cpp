#include <fstream>
#include <cstring>

#include "WavLoader.h"
#include "../Core/Log.h"

AudioTrack* WavLoader::LoadWavFileHeader(FileDataReader& reader)
{
    if (!reader.Read(4))
    {
        Log::LogError("Could not read RIFF");
        return nullptr;
    }
    if (std::strncmp(reader.Buffer(), "RIFF", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (header doesn't begin with RIFF)");
        return nullptr;
    }

    if (!reader.Read(4))
    {
        Log::LogError("Could not read size of file");
        return nullptr;
    }

    if (!reader.Read(4))
    {
        Log::LogError("Could not read WAVE");
        return nullptr;
    }
    if (std::strncmp(reader.Buffer(), "WAVE", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (header doesn't contain WAVE)");
        return nullptr;
    }

    if (!reader.Read(4))
    {
        Log::LogError("Could not read fmt/0");
        return nullptr;
    }

    if (!reader.Read(4))
    {
        Log::LogError("Could not read the 16");
        return nullptr;
    }

    if (!reader.Read(2))
    {
        Log::LogError("Could not read PCM");
        return nullptr;
    }

    if (!reader.Read(2))
    {
        Log::LogError("Could not read number of channels");
        return nullptr;
    }

    auto audioTrack = new AudioTrack();
    audioTrack->NumberOfChannels = ConvertToInt(reader.Buffer(), 2);

    if (!reader.Read(4))
    {
        Log::LogError("Could not read sample rate");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->SampleRate = ConvertToInt(reader.Buffer(), 4);

    if (!reader.Read(4))
    {
        Log::LogError("Could not read (sampleRate * bitsPerSample * channels) / 8");
        delete audioTrack;
        return nullptr;
    }

    if (!reader.Read(2))
    {
        Log::LogError("Could not read wave structure");
        delete audioTrack;
        return nullptr;
    }

    if (!reader.Read(2))
    {
        Log::LogError("Could not read bits per sample");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->BitsPerSample = ConvertToInt(reader.Buffer(), 2);

    if (!reader.Read(4))
    {
        Log::LogError("Could not read data chunk header");
        delete audioTrack;
        return nullptr;
    }
    if (std::strncmp(reader.Buffer(), "data", 4) != 0)
    {
        Log::LogError("File is not a valid WAVE file (doesn't have 'data' tag)");
        delete audioTrack;
        return nullptr;
    }

    if (!reader.Read(4))
    {
        Log::LogError("Could not read data size");
        delete audioTrack;
        return nullptr;
    }
    audioTrack->NumberOfSamples = ConvertToInt(reader.Buffer(), 4);

    if (reader.IsEnd())
    {
        Log::LogError("Reached EOF on the file");
        delete audioTrack;
        return nullptr;
    }

    return audioTrack;
}

AudioTrack* WavLoader::LoadWav(FileDataReader& reader)
{
    auto audioTrack = LoadWavFileHeader(reader);
    if (audioTrack == nullptr)
    {
        Log::LogError("Could not load wav header");
        return nullptr;
    }

    audioTrack->LengthInSeconds = (float)audioTrack->NumberOfSamples / (float)audioTrack->SampleRate;

    return audioTrack;
}
