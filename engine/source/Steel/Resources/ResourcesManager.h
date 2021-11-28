#pragma once

#include <vector>

#include "AsepriteData.h"
#include "ResourceID.h"
#include "Steel/Animation/Animation.h"
#include "Steel/Audio/AudioTrack.h"
#include "Steel/Rendering/MaterialSystem/Shader.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/UI/Font.h"

#ifdef DISTRIBUTE_BUILD
#define ENGINE_RESOURCES_PATH "Resources/"
#define RESOURCES_PATH "Resources/"
#else
#define ENGINE_RESOURCES_PATH "../../../engine/resources/"
#define RESOURCES_PATH "../../resources/"
#endif

class ResourcesManager
{
public:
    ResourcesManager();
    ~ResourcesManager();

    void LoadDefaultResources();
    const char* GetResourcesPath();

    Sprite* LoadSprite(const char* filePath, bool engineResource = false);
    void AddSprite(Sprite* sprite);
    Sprite* GetSprite(ResourceID imageID);
    void UnloadSprite(ResourceID imageID);

    void AddTexture(Texture* texture);
    Texture* GetTexture(ResourceID textureID);
    void UnloadTexture(ResourceID textureID);

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

    void AddShader(Shader* shader);
    Shader* GetShader(ResourceID shaderID);
    Shader* DefaultSpriteShader();
    Shader* DefaultUIShader();

    void AddMaterial(Material* material);
    Material* GetMaterial(ResourceID materialID);
    Material* DefaultSpriteMaterial();
    Material* DefaultUIMaterial();
    Material* DefaultUIClippingMaterial();

private:
    ResourceID nextResourceID = 0;
    ResourceID GetNextResourceID();
    void FreeResourceID(ResourceID resourceID);

    std::unordered_map<ResourceID, Sprite*> sprites;
    std::unordered_map<ResourceID, Texture*> textures;
    std::unordered_map<ResourceID, AudioTrack*> audioTracks;
    std::unordered_map<ResourceID, Animation*> animations;
    std::unordered_map<ResourceID, Font*> fonts;
    std::unordered_map<ResourceID, AsepriteData*> asepriteDatas;
    std::unordered_map<ResourceID, Shader*> shaders;
    std::unordered_map<ResourceID, Material*> materials;

    Font* defaultFont;
    Shader* defaultSpriteShader;
    Shader* defaultUIShader;
    Shader* defaultUIClippingShader;
    Material* defaultSpriteMaterial;
    Material* defaultUIMaterial;
    Material* defaultUIClippingMaterial;
};
