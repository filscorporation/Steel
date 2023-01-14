#include "AudioSource.h"
#include "AudioCore.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Serialization/AttributesRegistration.h"

void AudioSource::RegisterType()
{
    REGISTER_COMPONENT(AudioSource);
    REGISTER_ATTRIBUTE(AudioSource, "sourceVolume", GetVolume, SetVolume, float, AttributeFlags::Public, "Source volume");
    REGISTER_ATTRIBUTE(AudioSource, "isLoop", GetIsLoop, SetIsLoop, bool, AttributeFlags::Public, "Is loop");
}

void AudioSource::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::IsInitialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        sourceID = AudioCore::CreateSource();
        sourceCreated = true;

        ApplyAudioProperties();
    }
}

void AudioSource::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (sourceCreated)
    {
        AudioCore::DeleteSource(sourceID);
        sourceCreated = false;
    }
}

void AudioSource::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (AudioCore::IsInitialized())
    {
        sourceID = AudioCore::CreateSource();
        sourceCreated = true;

        ApplyAudioProperties();
    }
}

void AudioSource::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    if (sourceCreated)
    {
        AudioCore::DeleteSource(sourceID);
        sourceCreated = false;
    }
}

void AudioSource::OnUpdate()
{
    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.DidTransformationChange() && sourceCreated)
        AudioCore::SetSourcePosition(sourceID, transform.GetPosition());
}

void AudioSource::Play(AudioTrack* audioTrack)
{
    if (!sourceCreated)
    {
        Log::LogError("Audio source is not created");
        return;
    }

    if (audioTrack == nullptr)
    {
        Log::LogError("Audio track is null");
        return;
    }

    if (currentTrackID != audioTrack->BufferID)
    {
        AudioCore::SetSourceBuffer(sourceID, audioTrack->BufferID);
        currentTrackID = audioTrack->BufferID;
    }
    AudioCore::PlaySource(sourceID);
}

void AudioSource::Stop()
{
    if (!sourceCreated)
    {
        Log::LogError("Audio source is not created");
        return;
    }

    AudioCore::StopSource(sourceID);
}

bool AudioSource::GetIsLoop() const
{
    return sourceIsLoop;
}

void AudioSource::SetIsLoop(bool isLoop)
{
    sourceIsLoop = isLoop;

    if (sourceCreated && AudioCore::IsInitialized())
        AudioCore::SetSourceIsLoop(sourceID, isLoop);
}

float AudioSource::GetVolume() const
{
    return sourceVolume;
}

void AudioSource::SetVolume(float volume)
{
    sourceVolume = volume;
    if (sourceCreated && AudioCore::IsInitialized())
        AudioCore::SetSourceVolume(sourceID, volume);
}

void AudioSource::ApplyAudioProperties()
{
    SetIsLoop(sourceIsLoop);
    SetVolume(sourceVolume);
}
