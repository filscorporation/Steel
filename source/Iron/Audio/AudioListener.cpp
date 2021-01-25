#include "AudioListener.h"
#include "../Scene/Object.h"
#include "AudioSystem.h"

void AudioListener::OnUpdate()
{
    if (ParentObject->IsDestroyed())
        return;

    // TODO: check if changed
    AudioSystem::SetListenerPosition(ParentObject->Transform->Position);
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
