#pragma once

#include "../EntityComponentSystem/Component.h"
#include "AudioTrack.h"

class AudioSource : public Component
{
public:
    explicit AudioSource(EntityID ownerEntityID) : Component(ownerEntityID) { };

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;
    void OnUpdate();

    void Play(AudioTrack* audioTrack);
    void Stop();
    bool GetIsLoop();
    void SetIsLoop(bool isLoop);
    float GetVolume();
    void SetVolume(float volume);

private:
    uint32_t sourceID = 0;
    float sourceVolume = 1.0f;
    bool sourceIsLoop = false;
    ResourceID currentTrackID = NULL_RESOURCE;

    friend class AudioSystem;
};
