#include "AudioSource.h"
#include "AudioCore.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"

void AudioSource::RegisterType()
{
    REGISTER_TYPE(AudioSource);
    REGISTER_ATTRIBUTE(AudioSource, "sourceVolume", GetVolume, SetVolume, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(AudioSource, "isLoop", GetIsLoop, SetIsLoop, bool, AttributeFlags::Public);
}

void AudioSource::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    sourceID = AudioCore::CreateSource();
}

void AudioSource::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    AudioCore::DeleteSource(sourceID);
}

void AudioSource::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    sourceID = AudioCore::CreateSource();
}

void AudioSource::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    AudioCore::DeleteSource(sourceID);
}

void AudioSource::OnUpdate()
{
    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.DidTransformationChange())
        AudioCore::SetSourcePosition(sourceID, transform.GetPosition());
}

void AudioSource::Play(AudioTrack* audioTrack)
{
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
    AudioCore::StopSource(sourceID);
}

bool AudioSource::GetIsLoop() const
{
    return sourceIsLoop;
}

void AudioSource::SetIsLoop(bool isLoop)
{
    if (sourceIsLoop != isLoop)
    {
        sourceIsLoop = isLoop;
        AudioCore::SetSourceIsLoop(sourceID, isLoop);
    }
}

float AudioSource::GetVolume() const
{
    return sourceVolume;
}

void AudioSource::SetVolume(float volume)
{
    if (sourceVolume != volume)
    {
        sourceVolume = volume;
        AudioCore::SetSourceVolume(sourceID, volume);
    }
}
