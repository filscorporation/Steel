#include "AudioTrack.h"

#include <AL/al.h>

AudioTrack::AudioTrack()
{
    Type = ResourceTypes::AudioTrack;
}

AudioTrack::~AudioTrack()
{
    alDeleteBuffers(1, &BufferID);
}
