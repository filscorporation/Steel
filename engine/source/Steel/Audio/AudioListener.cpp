#include "AudioListener.h"
#include "AudioCore.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Scene/SceneHelper.h"

void AudioListener::RegisterType()
{
    REGISTER_TYPE(AudioListener);
    REGISTER_ATTRIBUTE(AudioListener, "listenerVolume", GetVolume, SetVolume, float, AttributeFlags::Public);
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
