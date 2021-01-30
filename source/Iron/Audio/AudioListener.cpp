#include "AudioListener.h"
#include "../Scene/Object.h"
#include "AudioSystem.h"

void AudioListener::OnUpdate()
{
    if (ParentObject->IsDestroyed())
        return;

    if (ParentObject->Transform->IsTransformationDirty())
        AudioSystem::SetListenerPosition(ParentObject->Transform->GetPosition());
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
