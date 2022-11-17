#include "AudioTrack.h"
#include "Steel/Serialization/AttributesRegistration.h"

#include <AL/al.h>

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
    alDeleteBuffers(1, &BufferID);
}
