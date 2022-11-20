#pragma once

#include <vector>

#include "AsepriteData.h"
#include "SceneData.h"
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
#define RESOURCE_DATA_EXTENSION ".data"

class ResourcesManager
{
public:
    ResourcesManager();
    ~ResourcesManager();

    const char* GetResourcesPath();

    void SaveResources();
    void LoadResources();
    void LoadDefaultResources();

    Resource* GetResource(const char* filePath);
    Resource* GetResource(ResourceTypes::ResourceType type, ResourceID resourceID);
    void AddResource(Resource* resource);
    bool ResourceExists(ResourceTypes::ResourceType type, ResourceID resourceID);
    void UnloadResource(ResourceTypes::ResourceType type, ResourceID resourceID);

    Sprite* LoadSprite(const char* filePath, bool engineResource = false);
    AsepriteData* LoadAsepriteData(const char* filePath, bool loopAll = false);
    AudioTrack* LoadAudioTrack(const char* filePath);
    Font* LoadFont(const char* fontPath, bool engineResource = false);
    Shader* LoadShader(const char* fileVSPath, const char* fileFSPath);
    SceneData* LoadSceneData(const char* filePath);

    Sprite* GetSprite(ResourceID imageID);
    Texture* GetTexture(ResourceID textureID);
    AsepriteData* GetAsepriteData(ResourceID resourceID);
    AudioTrack* GetAudioTrack(ResourceID audioID);
    Animation* GetAnimation(ResourceID animationID);
    Font* GetFont(ResourceID fontID);
    Shader* GetShader(ResourceID shaderID);
    SceneData* GetSceneData(ResourceID resourceID);

    int GetDefaultPixelsPerUnit();
    void SetDefaultPixelsPerUnit(int newValue);

    Font* DefaultFont();

    Shader* DefaultSpriteShader();
    Shader* DefaultUIShader();

    Material* GetMaterial(ResourceID materialID);
    Material* DefaultSpriteMaterial();
    Material* DefaultMeshMaterial();
    Material* DefaultUIMaterial();
    Material* DefaultUIClippingMaterial();

private:
    void TryLoadResource(const std::string& path);
    static std::string DataFilePath(const std::string& fileFullPath);

    std::vector<std::unordered_map<ResourceID, Resource*>> resources;

    int defaultPixelsPerUnit = 32;

    Font* defaultFont;
    Shader* defaultSpriteShader;
    Shader* defaultMeshShader;
    Shader* defaultUIShader;
    Shader* defaultUIClippingShader;
    Material* defaultSpriteMaterial;
    Material* defaultMeshMaterial;
    Material* defaultUIMaterial;
    Material* defaultUIClippingMaterial;
};
