#include "AudioListener.h"
#include "../Scene/Entity.h"
#include "AudioSystem.h"

void AudioListener::OnUpdate()
{
    if (ParentEntity->IsDestroyed())
        return;

    if (ParentEntity->Transform->IsTransformationDirty())
        AudioSystem::SetListenerPosition(ParentEntity->Transform->GetPosition());
}

void AudioListener::SetVolume(float volume)
{
    if (listenerVolume != volume)
    {
        listenerVolume = volume;
        AudioSystem::SetListenerVolume(volume);
    }
}

float AudioListener::GetVolume() const
{
    return listenerVolume;
}
