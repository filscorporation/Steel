#pragma once

#include <vector>
#include "ResourceID.h"
#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"
#include "../Audio/AudioTrack.h"

class ResourcesManager
{
public:
    ~ResourcesManager();

    Sprite* LoadImage(const char* filePath);
    Sprite* GetImage(ResourceID imageID);
    void UnloadImage(ResourceID imageID);

    AudioTrack* LoadAudioTrack(const char* filePath);
    AudioTrack* GetAudioTrack(ResourceID audioID);
    void UnloadAudioTrack(ResourceID audioID);

    void AddAnimation(Animation* animation);
    Animation* GetAnimation(ResourceID animationID);
    void RemoveAnimation(ResourceID animationID);

private:
    std::vector<Sprite*> images;
    std::vector<AudioTrack*> audioTracks;
    std::vector<Animation*> animations;
};
