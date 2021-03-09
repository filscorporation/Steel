#pragma once

#include "../EntityComponentSystem/Component.h"
#include "AudioTrack.h"

class AudioSource : public Component
{
public:
    explicit AudioSource(EntityID ownerEntityID);
    ~AudioSource() override;

    void OnUpdate();

    void Play(AudioTrack* audioTrack);
    void Stop();
    bool GetIsLoop();
    void SetIsLoop(bool isLoop);
    float GetVolume();
    void SetVolume(float volume);

private:
    unsigned int sourceID;
    float sourceVolume = 1.0f;
    bool sourceIsLoop = false;
};
