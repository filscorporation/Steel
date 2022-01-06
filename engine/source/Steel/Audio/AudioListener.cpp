#include "AudioListener.h"
#include "AudioCore.h"
#include "../Scene/Transformation.h"
#include "../Scene/SceneHelper.h"

void AudioListener::RegisterType()
{
    REGISTER_TYPE(AudioListener);
    // TODO
}

void AudioListener::OnUpdate()
{
    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.DidTransformationChange())
        AudioCore::SetListenerPosition(transform.GetPosition());
}

void AudioListener::SetVolume(float volume)
{
    if (listenerVolume != volume)
    {
        listenerVolume = volume;
        AudioCore::SetListenerVolume(volume);
    }
}

float AudioListener::GetVolume() const
{
    return listenerVolume;
}
