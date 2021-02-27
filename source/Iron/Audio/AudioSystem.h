#pragma once

#include "../Scene/Entity.h"

class AudioSystem
{
public:
    static bool Initialized();
    static void Init(Entity* listenerEntity);
    static void Terminate();
    static bool CheckForErrors();

    static void SetListenerPosition(glm::vec3 position);
    static void SetListenerOrientation(glm::vec3 at, glm::vec3 up);
    static void SetListenerVolume(float volume);
    static unsigned int CreateSource();
    static void DeleteSource(unsigned int sourceID);
    static void SetSourcePosition(unsigned int sourceID, glm::vec3 position);
    static void SetSourceIsLoop(unsigned int sourceID, bool isLoop);
    static void SetSourceVolume(unsigned int sourceID, float volume);
    static void SetSourceBuffer(unsigned int sourceID, unsigned int bufferID);
    static void PlaySource(unsigned int sourceID);

private:
    static bool AssertInitialized();
};
