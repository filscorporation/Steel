#include "AudioSource.h"
#include "AudioSystem.h"

AudioSource::AudioSource()
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
    AudioSystem::SetSourcePosition(sourceID, ParentObject->Transform->Position);
}

void AudioSource::Play(AudioTrack *audioTrack)
{
    AudioSystem::SetSourceBuffer(sourceID, audioTrack->BufferID);
    AudioSystem::PlaySource(sourceID);
}

void AudioSource::SetIsLoop(bool isLoop)
{
    if (sourceIsLoop != isLoop)
    {
        sourceIsLoop = isLoop;
        AudioSystem::SetSourceIsLoop(sourceID, isLoop);
    }
}

void AudioSource::SetVolume(float volume)
{
    if (sourceVolume != volume)
    {
        sourceVolume = volume;
        AudioSystem::SetSourceVolume(sourceID, volume);
    }
}
