#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class AudioListener : public Component
{
    COMPONENT(AudioListener)

public:
    explicit AudioListener(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnUpdate();

    void SetVolume(float volume);
    float GetVolume() const;

private:
    float listenerVolume = 1.0f;
};
