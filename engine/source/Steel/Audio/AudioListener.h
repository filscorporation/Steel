#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class AudioListener : public Component
{
    DEFINE_TYPE(AudioListener)

public:
    explicit AudioListener(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnUpdate();

    void SetVolume(float volume);
    float GetVolume() const;

private:
    void ApplyAudioProperties();

    float listenerVolume = 1.0f;
};
