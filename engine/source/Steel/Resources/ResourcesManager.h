#pragma once

#include "AsepriteData.h"
#include "SceneData.h"
#include "ResourceID.h"
#include "Steel/Animation/Animation.h"
#include "Steel/Audio/AudioTrack.h"
#include "Steel/Rendering/MaterialSystem/Shader.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/UI/Font.h"
#include "FilesManager.h"

#include <vector>
#include <filesystem>

#define RESOURCE_DATA_EXTENSION ".data"

class ResourcesManager
{
public:
    ResourcesManager();
    ~ResourcesManager();

    FilesManager* GetFilesManager() const;

    void SaveResources();
    void LoadResources(const std::string& folderPath);
    void LoadDefaultResources();

    static std::string PathToString(const std::filesystem::path& path);
    static std::string GetAbsolutePath(const std::string& path);
    static std::string GetAbsolutePath(const std::filesystem::path& path);
    static std::string GetResourcesPath();
    static std::string GetEngineResourcesPath();
    static std::filesystem::path GetResourceFilePath(const std::string& filePath);
    static std::filesystem::path GetEngineResourceFilePath(const std::string& filePath);
    Resource* TryLoadResource(const std::filesystem::path& filePath);
    Resource* GetResource(const std::string& filePath);
    Resource* GetResource(const std::filesystem::path& filePath);
    Resource* GetResource(ResourceTypes::ResourceType type, ResourceID resourceID);
    void AddResource(Resource* resource);
    bool ResourceExists(ResourceTypes::ResourceType type, ResourceID resourceID);
    void UnloadResource(ResourceTypes::ResourceType type, ResourceID resourceID);
    void UnloadResource(const std::string& filePath);

    Sprite* GetSprite(ResourceID imageID);
    Sprite* GetSprite(const std::string& filePath);
    Texture* GetTexture(ResourceID textureID);
    Texture* GetTexture(const std::string& filePath);
    AsepriteData* GetAsepriteData(ResourceID resourceID);
    AsepriteData* GetAsepriteData(const std::string& filePath, bool loopAll);
    AudioTrack* GetAudioTrack(ResourceID audioID);
    AudioTrack* GetAudioTrack(const std::string& filePath);
    Animation* GetAnimation(ResourceID animationID);
    Animation* GetAnimation(const std::string& filePath);
    Font* GetFont(ResourceID fontID);
    Font* GetFont(const std::string& filePath);
    Shader* GetShader(ResourceID shaderID);
    Shader* GetShader(const std::string& fileVSPath, const std::string& fileFSPath);
    SceneData* GetSceneData(ResourceID resourceID);
    SceneData* GetSceneData(const std::string& filePath);

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
    Sprite* LoadSprite(const std::filesystem::path& filePath);
    AsepriteData* LoadAsepriteData(const std::filesystem::path& filePath);
    AudioTrack* LoadAudioTrack(const std::filesystem::path& filePath);
    Font* LoadFont(const std::filesystem::path& filePath);
    Shader* LoadShader(const std::filesystem::path& fileVSPath, const std::filesystem::path& fileFSPath);
    SceneData* LoadSceneData(const std::filesystem::path& filePath);

    static std::string DataFilePath(const std::string& fileFullPath);
    static std::string ResourceTypeToString(ResourceTypes::ResourceType type);

    FilesManager* CreateFilesManager();

    FilesManager* filesManager = nullptr;

    std::vector<std::unordered_map<ResourceID, Resource*>> resources;
    std::unordered_map<std::string, Resource*> pathResourcesMap;

    int defaultPixelsPerUnit = 32;

    Font* defaultFont = nullptr;
    Shader* defaultSpriteShader = nullptr;
    Shader* defaultMeshShader = nullptr;
    Shader* defaultUIShader = nullptr;
    Shader* defaultUIClippingShader = nullptr;
    Material* defaultSpriteMaterial = nullptr;
    Material* defaultMeshMaterial = nullptr;
    Material* defaultUIMaterial = nullptr;
    Material* defaultUIClippingMaterial = nullptr;
};
