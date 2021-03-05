#pragma once

#include "../Scene/Component.h"
#include "AudioTrack.h"

class AudioSource : public Component
{
public:
    explicit AudioSource(Entity* parentEntity);
    ~AudioSource() override;

    void OnUpdate() override;

    void Play(AudioTrack* audioTrack);
    void SetIsLoop(bool isLoop);
    void SetVolume(float volume);

private:
    unsigned int sourceID;
    float sourceVolume = 1.0f;
    bool sourceIsLoop = false;
};
