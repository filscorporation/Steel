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
    Sprite* GetImage(unsigned int imageID);
    void UnloadImage(unsigned int imageID);

    AudioTrack* LoadAudioTrack(const char* filePath);
    AudioTrack* GetAudioTrack(unsigned int audioID);
    void UnloadAudioTrack(unsigned int audioID);

    void AddAnimation(Animation* animation);
    Animation* GetAnimation(unsigned int animationID);
    void RemoveAnimation(unsigned int animationID);

private:
    std::vector<Sprite*> images;
    std::vector<AudioTrack*> audioTracks;
    std::vector<Animation*> animations;
};
