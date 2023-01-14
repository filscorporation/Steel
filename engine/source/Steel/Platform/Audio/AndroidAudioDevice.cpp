#include "AndroidAudioDevice.h"

// TODO: init

void AndroidAudioDevice::Init()
{

}

void AndroidAudioDevice::Terminate()
{

}

bool AndroidAudioDevice::IsInitialized()
{
    return false;
}

bool AndroidAudioDevice::CheckForErrors()
{
    return false;
}

void AndroidAudioDevice::SetListenerPosition(glm::vec3 position)
{

}

void AndroidAudioDevice::SetListenerOrientation(glm::vec3 at, glm::vec3 up)
{

}

void AndroidAudioDevice::SetListenerVolume(float volume)
{

}

uint32_t AndroidAudioDevice::CreateSource()
{
    return 0;
}

void AndroidAudioDevice::DeleteSource(uint32_t sourceID)
{

}

void AndroidAudioDevice::SetSourcePosition(uint32_t sourceID, glm::vec3 position)
{

}

void AndroidAudioDevice::SetSourceIsLoop(uint32_t sourceID, bool isLoop)
{

}

void AndroidAudioDevice::SetSourceVolume(uint32_t sourceID, float volume)
{

}

void AndroidAudioDevice::SetSourceBuffer(uint32_t sourceID, int bufferID)
{

}

void AndroidAudioDevice::PlaySource(uint32_t sourceID)
{

}

void AndroidAudioDevice::StopSource(uint32_t sourceID)
{

}

bool AndroidAudioDevice::InitAudioTrack(AudioTrack* audioTrack, char* trackData)
{
    return false;
}

void AndroidAudioDevice::DeleteAudioTrack(AudioTrack* audioTrack)
{

}