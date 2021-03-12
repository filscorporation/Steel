#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "AudioSource.h"

class AudioSystem : public ComponentSystem<AudioSource>
{
public:
    void OnComponentAdded(EntityID entityID, AudioSource& component) override;
    void OnComponentRemoved(EntityID entityID, AudioSource& component) override;
};
