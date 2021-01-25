#pragma once

#include "../Scene/Component.h"

class AudioListener : Component
{
public:
    void OnUpdate() override;

    void SetVolume(float volume);
    float GetVolume() const;

private:
    float listenerVolume = 1.0f;
};
