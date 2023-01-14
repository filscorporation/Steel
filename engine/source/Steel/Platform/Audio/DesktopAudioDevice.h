#pragma once

#include "Steel/Audio/AudioDevice.h"
#include "Steel/Audio/AudioTrack.h"

class DesktopAudioDevice : public AudioDevice
{
public:
    DesktopAudioDevice() = default;
    ~DesktopAudioDevice() override = default;

    void Init() override;
    void Terminate() override;
    bool IsInitialized() override;
    bool CheckForErrors() override;

    void SetListenerPosition(glm::vec3 position) override;
    void SetListenerOrientation(glm::vec3 at, glm::vec3 up) override;
    void SetListenerVolume(float volume) override;
    uint32_t CreateSource() override;
    void DeleteSource(uint32_t sourceID) override;
    void SetSourcePosition(uint32_t sourceID, glm::vec3 position) override;
    void SetSourceIsLoop(uint32_t sourceID, bool isLoop) override;
    void SetSourceVolume(uint32_t sourceID, float volume) override;
    void SetSourceBuffer(uint32_t sourceID, int bufferID) override;
    void PlaySource(uint32_t sourceID) override;
    void StopSource(uint32_t sourceID) override;

    bool InitAudioTrack(AudioTrack* audioTrack, char* trackData) override;
    void DeleteAudioTrack(AudioTrack* audioTrack) override;
};
