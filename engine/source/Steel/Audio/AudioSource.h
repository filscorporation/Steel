#pragma once

#include "AudioTrack.h"
#include "Steel/EntityComponentSystem/Component.h"

class AudioSource : public Component
{
    DEFINE_TYPE(AudioSource)

public:
    explicit AudioSource(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;
    void OnUpdate();

    void Play(AudioTrack* audioTrack);
    void Stop();
    bool GetIsLoop() const;
    void SetIsLoop(bool isLoop);
    float GetVolume() const;
    void SetVolume(float volume);

private:
    void ApplyAudioProperties();

    uint32_t sourceID = 0;
    float sourceVolume = 1.0f;
    bool sourceIsLoop = false;
    ResourceID currentTrackID = NULL_RESOURCE;

    bool sourceCreated = false;
};
