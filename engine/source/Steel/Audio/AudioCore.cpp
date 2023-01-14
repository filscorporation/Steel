#include "AudioCore.h"
#include "AudioListener.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
#include "Steel/Platform/Audio/DesktopAudioDevice.h"
#endif
#if defined PLATFORM_ANDROID
#include "Steel/Platform/Audio/AndroidAudioDevice.h"
#endif

AudioDevice* AudioCore::audioDevice = nullptr;

void AudioCore::Init()
{
    if (IsInitialized())
    {
        Log::LogError("Error initializing audio system: already initialized");
        return;
    }

    audioDevice = CreateAudioDevice();
    audioDevice->Init();

    Log::LogDebug("Audio system initialized");
}

void AudioCore::Terminate()
{
    if (AssertInitialized())
        return;

    delete audioDevice;
}

bool AudioCore::IsInitialized()
{
    return audioDevice != nullptr && audioDevice->IsInitialized();
}

void AudioCore::CreateAudioScene(EntitiesRegistry* entitiesRegistry)
{
    if (AssertInitialized())
        return;
}

void AudioCore::CreateAudioListener(EntityID listenerEntity)
{
    if (AssertInitialized())
        return;

    AddComponentS<AudioListener>(listenerEntity);
    SetListenerPosition(GetComponentS<Transformation>(listenerEntity).GetPosition());
    SetListenerOrientation(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    SetListenerVolume(1.0f);
}

void AudioCore::DeleteAudioScene()
{
    if (AssertInitialized())
        return;
}

void AudioCore::SetListenerPosition(glm::vec3 position)
{
    if (AssertInitialized())
        return;

    audioDevice->SetListenerPosition(position);
}

void AudioCore::SetListenerOrientation(glm::vec3 at, glm::vec3 up)
{
    if (AssertInitialized())
        return;

    audioDevice->SetListenerOrientation(at, up);
}

void AudioCore::SetListenerVolume(float volume)
{
    if (AssertInitialized())
        return;

    audioDevice->SetListenerVolume(volume);
}

uint32_t AudioCore::CreateSource()
{
    if (AssertInitialized())
        return -1;

    return audioDevice->CreateSource();
}

void AudioCore::DeleteSource(uint32_t sourceID)
{
    if (AssertInitialized())
        return;

    audioDevice->DeleteSource(sourceID);
}

void AudioCore::SetSourcePosition(uint32_t sourceID, glm::vec3 position)
{
    if (AssertInitialized())
        return;

    audioDevice->SetSourcePosition(sourceID, position);
}

void AudioCore::SetSourceIsLoop(uint32_t sourceID, bool isLoop)
{
    if (AssertInitialized())
        return;

    audioDevice->SetSourceIsLoop(sourceID, isLoop);
}

void AudioCore::SetSourceVolume(uint32_t sourceID, float volume)
{
    if (AssertInitialized())
        return;

    audioDevice->SetSourceVolume(sourceID, volume);
}

void AudioCore::SetSourceBuffer(uint32_t sourceID, int bufferID)
{
    if (AssertInitialized())
        return;

    audioDevice->SetSourceBuffer(sourceID, bufferID);
}

void AudioCore::PlaySource(uint32_t sourceID)
{
    if (AssertInitialized())
        return;

    audioDevice->PlaySource(sourceID);
}

void AudioCore::StopSource(uint32_t sourceID)
{
    if (AssertInitialized())
        return;

    audioDevice->StopSource(sourceID);
}

bool AudioCore::InitAudioTrack(AudioTrack* audioTrack, char* trackData)
{
    if (AssertInitialized())
        return false;

    return audioDevice->InitAudioTrack(audioTrack, trackData);
}

void AudioCore::DeleteAudioTrack(AudioTrack* audioTrack)
{
    if (AssertInitialized())
        return;

    audioDevice->DeleteAudioTrack(audioTrack);
}

bool AudioCore::AssertInitialized()
{
    if (!IsInitialized())
    {
        Log::LogError("Audio system is not initialized");
        return true;
    }

    return false;
}

AudioDevice* AudioCore::CreateAudioDevice()
{
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    return new DesktopAudioDevice();
#endif
#if defined PLATFORM_ANDROID
    return new AndroidAudioDevice();
#endif
}