#include "AudioSystem.h"
#include "AudioListener.h"
#include "../Core/Log.h"

#include <AL/al.h>
#include <AL/alc.h>

// TODO: fixes openal low default volume
const float VOLUME_MULTIPLIER = 32.0f;

ALCdevice* device = nullptr;
ALCcontext* context = nullptr;

bool AudioSystem::Initialized()
{
    return context != nullptr;
}

bool AudioSystem::AssertInitialized()
{
    if (!Initialized())
    {
        Log::LogError("Audio system is not initialized");
        return true;
    }

    return false;
}

void AudioSystem::Init(Object* listenerObject)
{
    if (Initialized())
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

    listenerObject->AddComponent<AudioListener>();
    SetListenerPosition(listenerObject->Transform->GetPosition());
    SetListenerOrientation(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    SetListenerVolume(1.0f);

    Log::LogInfo("Audio system initialized");
}

void AudioSystem::Terminate()
{
    if (AssertInitialized())
        return;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool AudioSystem::CheckForErrors()
{
    ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        switch(error)
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

void AudioSystem::SetListenerPosition(glm::vec3 position)
{
    if (AssertInitialized())
        return;

    alListener3f(AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener position");
    }
}

void AudioSystem::SetListenerOrientation(glm::vec3 at, glm::vec3 up)
{
    if (AssertInitialized())
        return;

    float value[6] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, (ALfloat*)value);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener orientation");
    }
}

void AudioSystem::SetListenerVolume(float volume)
{
    if (AssertInitialized())
        return;

    alListenerf(AL_GAIN, (ALfloat)volume * VOLUME_MULTIPLIER);
    if (CheckForErrors())
    {
        Log::LogError("Error setting listener volume");
    }
}

unsigned int AudioSystem::CreateSource()
{
    if (AssertInitialized())
        return -1;

    ALuint source;

    alGenSources((ALuint)1, &source);
    if (CheckForErrors())
    {
        Log::LogError("Error creating audio source");
        return -1;
    }

    return source;
}

void AudioSystem::DeleteSource(unsigned int sourceID)
{
    if (AssertInitialized())
        return;

    alDeleteSources(1, (ALuint*)&sourceID);
    if (CheckForErrors())
    {
        Log::LogError("Error deleting source");
    }
}

void AudioSystem::SetSourcePosition(unsigned int sourceID, glm::vec3 position)
{
    if (AssertInitialized())
        return;

    alSource3f(sourceID, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source position");
    }
}

void AudioSystem::SetSourceIsLoop(unsigned int sourceID, bool isLoop)
{
    if (AssertInitialized())
        return;

    alSourcei(sourceID, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source is loop");
    }
}

void AudioSystem::SetSourceVolume(unsigned int sourceID, float volume)
{
    if (AssertInitialized())
        return;

    alSourcef(sourceID, AL_GAIN, (ALfloat)volume);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source volume");
    }
}

void AudioSystem::SetSourceBuffer(unsigned int sourceID, unsigned int bufferID)
{
    if (AssertInitialized())
        return;

    alSourcei(sourceID, AL_BUFFER, (ALuint)bufferID);
    if (CheckForErrors())
    {
        Log::LogError("Error setting source bugger");
    }
}

void AudioSystem::PlaySource(unsigned int sourceID)
{
    if (AssertInitialized())
        return;

    alSourcePlay((ALuint)sourceID);
    if (CheckForErrors())
    {
        Log::LogError("Error playing from source");
    }
}
