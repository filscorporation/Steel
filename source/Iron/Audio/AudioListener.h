#pragma once

#include "../EntityComponentSystem/Component.h"

class AudioListener : public Component
{
public:
    explicit AudioListener(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnUpdate();

    void SetVolume(float volume);
    float GetVolume() const;

private:
    float listenerVolume = 1.0f;
};
