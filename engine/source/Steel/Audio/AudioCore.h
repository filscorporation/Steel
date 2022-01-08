#pragma once

#include <glm/glm.hpp>

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "AudioTrack.h"

class AudioCore
{
public:
    static bool Initialized();
    static void Init();
    static void Terminate();
    static void CreateAudioScene(EntitiesRegistry* entitiesRegistry);
    static void CreateAudioListener(EntityID listenerEntity);
    static void DeleteAudioScene();
    static bool CheckForErrors();

    static void SetListenerPosition(glm::vec3 position);
    static void SetListenerOrientation(glm::vec3 at, glm::vec3 up);
    static void SetListenerVolume(float volume);
    static uint32_t CreateSource();
    static void DeleteSource(uint32_t sourceID);
    static void SetSourcePosition(uint32_t sourceID, glm::vec3 position);
    static void SetSourceIsLoop(uint32_t sourceID, bool isLoop);
    static void SetSourceVolume(uint32_t sourceID, float volume);
    static void SetSourceBuffer(uint32_t sourceID, int bufferID);
    static void PlaySource(uint32_t sourceID);
    static void StopSource(uint32_t sourceID);

    static bool InitAudioTrack(AudioTrack* audioTrack, char* trackData);

private:
    static bool AssertInitialized();
};
