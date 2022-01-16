#include "AudioListener.h"
#include "AudioCore.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Scene/SceneHelper.h"

void AudioListener::RegisterType()
{
    REGISTER_TYPE(AudioListener);
    REGISTER_ATTRIBUTE(AudioListener, "listenerVolume", GetVolume, SetVolume, float, AttributeFlags::Public);
}

void AudioListener::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::Initialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        ApplyAudioProperties();
    }
}

void AudioListener::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::Initialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        ApplyAudioProperties();
    }
}

void AudioListener::OnUpdate()
{
    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.DidTransformationChange() && AudioCore::Initialized())
        AudioCore::SetListenerPosition(transform.GetPosition());
}

void AudioListener::SetVolume(float volume)
{
    listenerVolume = volume;
    if (AudioCore::Initialized())
        AudioCore::SetListenerVolume(volume);
}

float AudioListener::GetVolume() const
{
    return listenerVolume;
}

void AudioListener::ApplyAudioProperties()
{
    SetVolume(listenerVolume);
}
