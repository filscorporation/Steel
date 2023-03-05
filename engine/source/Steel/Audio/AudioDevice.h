#pragma once

#include "Steel/Audio/AudioTrack.h"

#include "glm/vec3.hpp"

class AudioDevice
{
public:
    AudioDevice() = default;
    virtual ~AudioDevice() = default;

    virtual void Init() = 0;
    virtual void Terminate() = 0;
    virtual bool IsInitialized() = 0;
    virtual bool CheckForErrors() = 0;

    virtual void SetListenerPosition(glm::vec3 position) = 0;
    virtual void SetListenerOrientation(glm::vec3 at, glm::vec3 up) = 0;
    virtual void SetListenerVolume(float volume) = 0;
    virtual uint32_t CreateSource() = 0;
    virtual void DeleteSource(uint32_t sourceID) = 0;
    virtual void SetSourcePosition(uint32_t sourceID, glm::vec3 position) = 0;
    virtual void SetSourceIsLoop(uint32_t sourceID, bool isLoop) = 0;
    virtual void SetSourceVolume(uint32_t sourceID, float volume) = 0;
    virtual void SetSourceBuffer(uint32_t sourceID, int bufferID) = 0;
    virtual void PlaySource(uint32_t sourceID) = 0;
    virtual void StopSource(uint32_t sourceID) = 0;

    virtual bool InitAudioTrack(AudioTrack* audioTrack, const char* trackData) = 0;
    virtual void DeleteAudioTrack(AudioTrack* audioTrack) = 0;
};
