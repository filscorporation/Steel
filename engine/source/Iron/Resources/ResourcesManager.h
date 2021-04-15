#pragma once

#include <vector>

#include "AsepriteData.h"
#include "ResourceID.h"
#include "../Animation/Animation.h"
#include "../Audio/AudioTrack.h"
#include "../Rendering/Sprite.h"
#include "../UI/Font.h"

class ResourcesManager
{
public:
    ResourcesManager();
    ~ResourcesManager();

    void LoadDefaultFont();
    const char* GetResourcesPath();

    Sprite* LoadImage(const char* filePath, bool engineResource = false);
    void AddImage(Sprite* image);
    Sprite* GetImage(ResourceID imageID);
    void UnloadImage(ResourceID imageID);
    static bool IsImageTransparent(const unsigned char* imageData, uint32_t width, uint32_t height);

    AsepriteData* LoadAsepriteData(const char* filePath, bool loopAll);
    AsepriteData* GetAsepriteData(ResourceID resourceID);

    AudioTrack* LoadAudioTrack(const char* filePath);
    AudioTrack* GetAudioTrack(ResourceID audioID);
    void UnloadAudioTrack(ResourceID audioID);

    void AddAnimation(Animation* animation);
    Animation* GetAnimation(ResourceID animationID);
    void RemoveAnimation(ResourceID animationID);

    Font* LoadFont(const char* fontPath, bool engineResource = false);
    Font* GetFont(ResourceID fontID);
    Font* DefaultFont();

private:

    std::vector<Sprite*> images;
    std::vector<AudioTrack*> audioTracks;
    std::vector<Animation*> animations;
    std::vector<Font*> fonts;
    std::vector<AsepriteData*> asepriteDatas;

    Font* defaultFont;
};
