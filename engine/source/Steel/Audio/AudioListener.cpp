#include "AudioListener.h"
#include "AudioCore.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Serialization/AttributesRegistration.h"

void AudioListener::RegisterType()
{
    REGISTER_COMPONENT(AudioListener);
    REGISTER_ATTRIBUTE(AudioListener, "listenerVolume", GetVolume, SetVolume, float, AttributeFlags::Public, "Listener volume");
}

void AudioListener::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::IsInitialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        ApplyAudioProperties();
    }
}

void AudioListener::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::IsInitialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        ApplyAudioProperties();
    }
}

void AudioListener::OnUpdate()
{
    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.DidTransformationChange() && AudioCore::IsInitialized())
        AudioCore::SetListenerPosition(transform.GetPosition());
}

void AudioListener::SetVolume(float volume)
{
    listenerVolume = volume;
    if (AudioCore::IsInitialized())
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
