#pragma once

#include "../Scene/Component.h"

class AudioListener : public Component
{
public:
    explicit AudioListener(Entity* parentEntity) : Component(parentEntity) { }

    void OnUpdate() override;

    void SetVolume(float volume);
    float GetVolume() const;

private:
    float listenerVolume = 1.0f;
};
