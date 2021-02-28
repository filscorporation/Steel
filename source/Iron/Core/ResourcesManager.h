#pragma once

#include <vector>
#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"
#include "../Audio/AudioTrack.h"

class ResourcesManager
{
public:
    ~ResourcesManager();

    Sprite* LoadImage(const char* filePath);
    Sprite* GetImage(uint64_t imageID);
    void UnloadImage(uint64_t imageID);

    AudioTrack* LoadAudioTrack(const char* filePath);
    AudioTrack* GetAudioTrack(uint64_t audioID);
    void UnloadAudioTrack(uint64_t audioID);

    void AddAnimation(Animation* animation);
    Animation* GetAnimation(uint64_t animationID);
    void RemoveAnimation(uint64_t animationID);

private:
    std::vector<Sprite*> images;
    std::vector<AudioTrack*> audioTracks;
    std::vector<Animation*> animations;
};
