#include "AudioTrack.h"
#include "Steel/Serialization/AttributesRegistration.h"
#include "AudioCore.h"

void AudioTrack::RegisterType()
{
    REGISTER_TYPE(AudioTrack);
}

AudioTrack::AudioTrack()
{
    Type = ResourceTypes::AudioTrack;
}

AudioTrack::~AudioTrack()
{
    AudioCore::DeleteAudioTrack(this);
}
