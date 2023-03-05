#include "DesktopAudioDevice.h"
#include "Steel/Core/Log.h"

#include <AL/al.h>
#include <AL/alc.h>

// Fixing openal low default volume
const float VOLUME_MULTIPLIER = 8.0f;

ALCdevice* device = nullptr;
ALCcontext* context = nullptr;

void DesktopAudioDevice::Init()
{
    if (IsInitialized())
    {
        Log::LogError("Error initializing audio system: already initialized");
        return;
    }

    device = alcOpenDevice(nullptr);
    if (device == nullptr)
    {
        Log::LogError("Error initializing audio system: no device found");
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (context == nullptr)
    {
        Log::LogError("Error initializing audio system: can't create context");
        alcCloseDevice(device);
        return;
    }
    if (!alcMakeContextCurrent(context))
    {
        Log::LogError("Error initializing audio system: can't make context current");
        alcCloseDevice(device);
        return;
    }
    if (CheckForErrors())
    {
        Log::LogError("Error initializing audio system: inner error");
        alcCloseDevice(device);
        return;
    }
}

void DesktopAudioDevice::Terminate()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool DesktopAudioDevice::IsInitialized()
{
    return context != nullptr;
}

bool DesktopAudioDevice::CheckForErrors()
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        switch (error)
        {
            case AL_INVALID_NAME:
                Log::LogError("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
                break;
            case AL_INVALID_ENUM:
                Log::LogError("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
                break;
            case AL_INVALID_VALUE:
                Log::LogError("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                break;
            case AL_INVALID_OPERATION:
                Log::LogError("AL_INVALID_OPERATION: the requested operation is not valid");
                break;
            case AL_OUT_OF_MEMORY:
                Log::LogError("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
                break;
            default:
                Log::LogError("UNKNOWN AL ERROR: ");
        }
        return true;
    }
    return false;
}

void DesktopAudioDevice::SetListenerPosition(glm::vec3 position)
{
    alListener3f(AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener position");
    }
}

void DesktopAudioDevice::SetListenerOrientation(glm::vec3 at, glm::vec3 up)
{
    float value[6] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, (ALfloat*)value);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener orientation");
    }
}

void DesktopAudioDevice::SetListenerVolume(float volume)
{
    alListenerf(AL_GAIN, (ALfloat)volume * VOLUME_MULTIPLIER);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener volume");
    }
}

uint32_t DesktopAudioDevice::CreateSource()
{
    ALuint source;

    alGenSources((ALuint)1, &source);
    if (CheckForErrors())
    {
        Log::LogError("Error creating audio source");
        return -1;
    }

    return source;
}

void DesktopAudioDevice::DeleteSource(uint32_t sourceID)
{
    alDeleteSources(1, (ALuint*)&sourceID);
    if (CheckForErrors())
    {
        Log::LogError("Error deleting source {0}", sourceID);
    }
}

void DesktopAudioDevice::SetSourcePosition(uint32_t sourceID, glm::vec3 position)
{
    alSource3f(sourceID, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source position {0}", sourceID);
    }
}

void DesktopAudioDevice::SetSourceIsLoop(uint32_t sourceID, bool isLoop)
{
    alSourcei(sourceID, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source is loop {0}", sourceID);
    }
}

void DesktopAudioDevice::SetSourceVolume(uint32_t sourceID, float volume)
{
    alSourcef(sourceID, AL_GAIN, (ALfloat)volume);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source volume {0}", sourceID);
    }
}

void DesktopAudioDevice::SetSourceBuffer(uint32_t sourceID, int bufferID)
{
    alSourceStop((ALuint)sourceID);
    alSourcei(sourceID, AL_BUFFER, (ALint)bufferID);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source buffer {0}", sourceID);
    }
}

void DesktopAudioDevice::PlaySource(uint32_t sourceID)
{
    alSourcePlay((ALuint)sourceID);
    if (CheckForErrors())
    {
        Log::LogError("Error playing from source {0}", sourceID);
    }
}

void DesktopAudioDevice::StopSource(uint32_t sourceID)
{
    alSourceStop((ALuint)sourceID);
    if (CheckForErrors())
    {
        Log::LogError("Error stopping source {0}", sourceID);
    }
}

static inline ALenum ToALFormat(int channels, int samples)
{
    bool stereo = (channels > 1);

    switch (samples)
    {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
    }
}

bool DesktopAudioDevice::InitAudioTrack(AudioTrack* audioTrack, const char* trackData)
{
    ALuint audioBuffer;
    alGenBuffers((ALuint)1, &audioBuffer);
    if (DesktopAudioDevice::CheckForErrors())
    {
        Log::LogError("Error generating audio buffer");

        return false;
    }

    alBufferData(audioBuffer, ToALFormat(audioTrack->NumberOfChannels, audioTrack->BitsPerSample),
                 trackData, (ALsizei)audioTrack->NumberOfSamples, audioTrack->SampleRate);

    if (DesktopAudioDevice::CheckForErrors())
    {
        Log::LogError("Error loading audio data to buffer");

        return false;
    }

    audioTrack->BufferID = audioBuffer;

    return true;
}

void DesktopAudioDevice::DeleteAudioTrack(AudioTrack* audioTrack)
{
    alDeleteBuffers(1, &(audioTrack->BufferID));
}