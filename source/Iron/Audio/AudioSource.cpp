#include "AudioSource.h"
#include "AudioSystem.h"
#include "../Core/Log.h"

AudioSource::AudioSource(Entity* parentEntity) : Component(parentEntity)
{
    sourceID = AudioSystem::CreateSource();
}

AudioSource::~AudioSource()
{
    AudioSystem::DeleteSource(sourceID);
}

void AudioSource::OnUpdate()
{
    // TODO: check if changed
    AudioSystem::SetSourcePosition(sourceID, ParentEntity->Transform->GetPosition());
}

void AudioSource::Play(AudioTrack *audioTrack)
{
    if (audioTrack == nullptr)
    {
        Log::LogError("Audio track is null");
        return;
    }

    AudioSystem::SetSourceBuffer(sourceID, audioTrack->BufferID);
    AudioSystem::PlaySource(sourceID);
}

void AudioSource::Stop()
{
    AudioSystem::StopSource(sourceID);
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
        AudioSystem::SetSourceIsLoop(sourceID, isLoop);
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
        AudioSystem::SetSourceVolume(sourceID, volume);
    }
}
