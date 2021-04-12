#include "AudioSource.h"
#include "AudioCore.h"
#include "../Core/Log.h"
#include "../Scene/SceneHelper.h"

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

bool AudioSource::GetIsLoop()
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

float AudioSource::GetVolume()
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
