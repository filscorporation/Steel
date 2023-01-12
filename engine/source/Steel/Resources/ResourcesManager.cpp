#include "ResourcesManager.h"
#include "AsepriteLoader.h"
#include "PngLoader.h"
#include "Steel/Core/Log.h"
#include "Steel/Math/Math.h"
#include "Steel/Math/Random.h"
#include "Steel/Audio/AudioCore.h"
#include "Steel/Audio/WavLoader.h"
#include "Steel/Rendering/MaterialSystem/BuiltInShaders.h"
#include "Steel/Serialization/SerializationManager.h"
#include "Steel/UI/FontManager.h"

#include <yaml-cpp/yaml.h>

ResourcesManager::ResourcesManager()
{
    resources.resize(RESOURCE_TYPES_COUNT);

    FontManager::Init();
}

ResourcesManager::~ResourcesManager()
{
    for (const auto& resourceStorage : resources)
    {
        for (auto resource : resourceStorage)
        {
            delete resource.second;
        }
    }
    resources.clear();

    FontManager::Terminate();
}

void ResourcesManager::SaveResources()
{
    for (const auto& resourceStorage : resources)
    {
        for (auto resource : resourceStorage)
        {
            if (std::filesystem::exists(resource.second->FullPath))
            {
                // TODO: check if changed
                SerializationManager::SerializeResource(resource.second, DataFilePath(resource.second->FullPath));
            }
        }
    }
}

std::string ResourcesManager::DataFilePath(const std::string& fileFullPath)
{
    return fileFullPath + RESOURCE_DATA_EXTENSION;
}

std::string ResourcesManager::ResourceTypeToString(ResourceTypes::ResourceType type)
{
    switch(type)
    {
        case ResourceTypes::Undefined:
            return "Undefined";
        case ResourceTypes::Sprite:
            return "Sprite";
        case ResourceTypes::Texture:
            return "Texture";
        case ResourceTypes::AudioTrack:
            return "AudioTrack";
        case ResourceTypes::Animation:
            return "Animation";
        case ResourceTypes::Font:
            return "Font";
        case ResourceTypes::AsepriteData:
            return "AsepriteData";
        case ResourceTypes::Shader:
            return "Shader";
        case ResourceTypes::Material:
            return "Material";
        case ResourceTypes::SceneData:
            return "SceneData";
    }

    return "";
}

void ResourcesManager::LoadResources(const std::string& folderPath)
{
    if (!std::filesystem::exists(folderPath))
    {
        Log::LogWarning("Resources not found in path: \"{0}\"", folderPath);
        return;
    }

    for (std::filesystem::recursive_directory_iterator i(folderPath), end; i != end; ++i)
        if (!is_directory(i->path()) && is_regular_file(i->path()))
            TryLoadResource(GetAbsolutePath(i->path()));
}

Resource* ResourcesManager::TryLoadResource(const std::filesystem::path& filePath)
{
    if (pathResourcesMap.find(PathToString(filePath)) != pathResourcesMap.end())
    {
        Log::LogWarning("Trying to load existing resource {0}", PathToString(filePath));
        return nullptr;
    }

    Resource* resource = nullptr;
    std::string extension = PathToString(filePath.extension());
    if (extension == ".png")
        resource = LoadSprite(filePath);
    else if (extension == ".wav")
        resource = LoadAudioTrack(filePath);
    else if (extension == ".ttf")
        resource = LoadFont(filePath);
    else if (extension == ".aseprite")
        resource = LoadAsepriteData(filePath);
    else if (extension == ".vs")
    {
        // TODO: combine shaders into one file
        std::string fsPath = PathToString(filePath);
        fsPath.replace(fsPath.length() - 2, 2, "fs");
        std::ifstream infile(fsPath);
        if (infile.good())
            // Load only if there is fragment shader file with the same name
            resource = LoadShader(filePath, fsPath);
    }
    else if (extension == ".scene")
        resource = LoadSceneData(filePath);

    if (resource == nullptr)
        return nullptr;

    resource->FullPath = PathToString(filePath);
    SerializationManager::DeserializeResource(resource, DataFilePath(resource->FullPath));

    AddResource(resource);

    Log::LogDebug("Loaded {0}: {1} ({2})", ResourceTypeToString(resource->Type), resource->FullPath, resource->ID);

    return resource;
}

void ResourcesManager::LoadDefaultResources()
{
    defaultFont = (Font*)GetResource(GetEngineResourceFilePath("font.ttf"));
    if (defaultFont != nullptr)
        defaultFont->AddSizeIfNotExists(32);

    defaultSpriteShader = new Shader(BuiltInShaders::DefaultSpriteVS, BuiltInShaders::DefaultSpriteFS);
    defaultSpriteShader->ID = 2067751924167351524u;
    AddResource(defaultSpriteShader);

    defaultMeshShader = new Shader(BuiltInShaders::DefaultMeshVS, BuiltInShaders::DefaultMeshFS);
    defaultMeshShader->ID = 9333321171270516576u;
    AddResource(defaultMeshShader);

    defaultUIShader = new Shader(BuiltInShaders::DefaultUIVS, BuiltInShaders::DefaultUIFS);
    defaultUIShader->ID = 7517276725264868109u;
    AddResource(defaultUIShader);

    defaultUIClippingShader = new Shader(BuiltInShaders::DefaultUIClippingVS, BuiltInShaders::DefaultUIClippingFS);
    defaultUIClippingShader->ID = 11264929139740125489u;
    AddResource(defaultUIClippingShader);

    defaultSpriteMaterial = new Material();
    defaultSpriteMaterial->MainShader = defaultSpriteShader;
    defaultSpriteMaterial->ID = 10625867161732373110u;
    AddResource(defaultSpriteMaterial);

    defaultMeshMaterial = new Material();
    defaultMeshMaterial->MainShader = defaultMeshShader;
    defaultMeshMaterial->ID = 14461234298104667705u;
    AddResource(defaultMeshMaterial);

    defaultUIMaterial = new Material();
    defaultUIMaterial->MainShader = defaultUIShader;
    defaultUIMaterial->ID = 10333421371436349153u;
    AddResource(defaultUIMaterial);

    defaultUIClippingMaterial = new Material();
    defaultUIClippingMaterial->MainShader = defaultUIClippingShader;
    defaultUIClippingMaterial->ID = 16469355927102697336u;
    AddResource(defaultUIClippingMaterial);
}

const char* ResourcesManager::GetResourcesPath()
{
    return RESOURCES_PATH;
}

bool ResourcesManager::ResourceExists(ResourceTypes::ResourceType type, ResourceID resourceID)
{
    return resourceID != NULL_RESOURCE && type != ResourceTypes::Undefined
        && resources[(int)type].find(resourceID) != resources[(int)type].end();
}

void ResourcesManager::AddResource(Resource* resource)
{
    if (resource->Type == ResourceTypes::Undefined)
    {
        Log::LogError("Error adding resource: type is undefined");
        return;
    }

    if (resource->ID == NULL_RESOURCE)
        resource->ID = Random::NextULong();

    if (resources[(int)resource->Type].find(resource->ID) != resources[(int)resource->Type].end())
    {
        Log::LogWarning("Replacing existing resource {0} ({1})", resource->ID, ResourceTypeToString(resource->Type));
        UnloadResource(resource->Type, resource->ID);
    }

    resources[(int)resource->Type][resource->ID] = resource;
    if (!resource->FullPath.empty())
    {
        if (pathResourcesMap.find(resource->FullPath) != pathResourcesMap.end())
            Log::LogWarning("Replacing existing resource in path map {0}", resource->FullPath);
        pathResourcesMap[resource->FullPath] = resource;
    }
}

std::string ResourcesManager::PathToString(const std::filesystem::path& path)
{
    return path.u8string();
}

std::string ResourcesManager::GetAbsolutePath(const std::string& path)
{
    return GetAbsolutePath(std::filesystem::path(path));
}

std::string ResourcesManager::GetAbsolutePath(const std::filesystem::path& path)
{
    return weakly_canonical(path).string();
}

std::filesystem::path ResourcesManager::GetResourceFilePath(const std::string& filePath)
{
    return GetAbsolutePath(RESOURCES_PATH + filePath);
}

std::filesystem::path ResourcesManager::GetEngineResourceFilePath(const std::string& filePath)
{
    return GetAbsolutePath(ENGINE_RESOURCES_PATH + filePath);
}

Resource* ResourcesManager::GetResource(const std::string& filePath)
{
    if (pathResourcesMap.find(filePath) != pathResourcesMap.end())
        return pathResourcesMap[filePath];
    return nullptr;
}

Resource* ResourcesManager::GetResource(const std::filesystem::path& filePath)
{
    return GetResource(PathToString(filePath));
}

Resource* ResourcesManager::GetResource(ResourceTypes::ResourceType type, ResourceID resourceID)
{
    if (!ResourceExists(type, resourceID))
        return nullptr;

    return resources[(int)type][resourceID];
}

void ResourcesManager::UnloadResource(ResourceTypes::ResourceType type, ResourceID resourceID)
{
    auto resource = GetResource(type, resourceID);
    if (resource == nullptr)
        return;

    resources[(int)type].erase(resourceID);

    if (!resource->FullPath.empty() && pathResourcesMap.find(resource->FullPath) != pathResourcesMap.end())
        pathResourcesMap.erase(resource->FullPath);

    delete resource;
}

void ResourcesManager::UnloadResource(const std::string& filePath)
{
    auto resource = GetResource(filePath);
    if (resource == nullptr)
        return;

    Log::LogDebug("Unloaded {0}: {1} ({2})", ResourceTypeToString(resource->Type), resource->FullPath, resource->ID);

    resources[(int)resource->Type].erase(resource->ID);

    pathResourcesMap.erase(filePath);

    delete resource;
}

inline std::string GetNameFromPath(const std::string& path)
{
    auto filename = std::filesystem::path(path).filename();
    auto filenameString = ResourcesManager::PathToString(filename);
    return filenameString.substr(0, filenameString.size() - ResourcesManager::PathToString(filename.extension()).size());
}

Sprite* ResourcesManager::LoadSprite(const std::filesystem::path& filePath)
{
    if (!std::ifstream(filePath).good())
    {
        Log::LogError("Error loading image: file {0} does not exist", PathToString(filePath));
        return nullptr;
    }

    Sprite* image;
    std::string extension = PathToString(filePath.extension());
    if (extension == ".png")
    {
        image = PngLoader::LoadImage(PathToString(filePath).c_str());
    }
    else if (extension == ".aseprite")
    {
        auto data = AsepriteLoader::LoadAsepriteData(PathToString(filePath).c_str());
        if (data == nullptr)
            return nullptr;

        data->Name = GetNameFromPath(PathToString(filePath));

        if (data->Sprites.empty())
        {
            Log::LogError("Error loading image: no data in aseprite file");
            return nullptr;
        }
        if (data->Sprites.size() > 1)
            Log::LogWarning("Aseprite file contains multiple images but only first will be used");

        image = data->Sprites[0];
        delete data;
    }
    else
    {
        Log::LogError("Error loading image: .{0} files not supported", extension);
        return nullptr;
    }

    image->SetPixelsPerUnit(defaultPixelsPerUnit);

    return image;
}

Sprite* ResourcesManager::GetSprite(ResourceID imageID)
{
    return (Sprite*)(GetResource(ResourceTypes::Sprite, imageID));
}

Sprite* ResourcesManager::GetSprite(const std::string& filePath)
{
    return (Sprite*)GetResource(GetResourceFilePath(filePath));
}

Texture* ResourcesManager::GetTexture(ResourceID textureID)
{
    return (Texture*)(GetResource(ResourceTypes::Texture, textureID));
}

Texture* ResourcesManager::GetTexture(const std::string& filePath)
{
    return (Texture*)GetResource(GetResourceFilePath(filePath));
}

AsepriteData* ResourcesManager::LoadAsepriteData(const std::filesystem::path& filePath)
{
    if (!std::ifstream(filePath).good())
    {
        Log::LogError("Error loading aseprite data: file {0} does not exist", PathToString(filePath));
        return nullptr;
    }

    std::string extension = PathToString(filePath.extension());
    if (extension != ".aseprite")
    {
        Log::LogError("Error loading aseprite file: .{0} files not supported", extension);
        return nullptr;
    }

    auto data = AsepriteLoader::LoadAsepriteData(PathToString(filePath).c_str());
    if (data == nullptr)
        return nullptr;

    data->Name = GetNameFromPath(PathToString(filePath));

    for (auto sprite : data->Sprites)
        sprite->SetPixelsPerUnit(defaultPixelsPerUnit);

    return data;
}

AsepriteData* ResourcesManager::GetAsepriteData(ResourceID resourceID)
{
    return (AsepriteData*)(GetResource(ResourceTypes::AsepriteData, resourceID));
}

AsepriteData* ResourcesManager::GetAsepriteData(const std::string& filePath, bool loopAll)
{
    auto data = (AsepriteData*)GetResource(GetResourceFilePath(filePath));
    if (loopAll)
        for (auto animation : data->Animations)
            animation->Loop = true;
    return data;
}

AudioTrack* ResourcesManager::LoadAudioTrack(const std::filesystem::path& filePath)
{
    if (!std::ifstream(filePath).good())
    {
        Log::LogError("Error loading audio track: file {0} does not exist", PathToString(filePath));
        return nullptr;
    }

    char* data;
    AudioTrack* audioTrack = WavLoader::LoadWav(PathToString(filePath).c_str(), &data);
    if (audioTrack == nullptr)
        return nullptr;

    if (!AudioCore::InitAudioTrack(audioTrack, data))
    {
        delete[] data;
        delete audioTrack;
        return nullptr;
    }
    delete[] data;

    return audioTrack;
}

AudioTrack* ResourcesManager::GetAudioTrack(ResourceID audioID)
{
    return (AudioTrack*)(GetResource(ResourceTypes::AudioTrack, audioID));
}

AudioTrack* ResourcesManager::GetAudioTrack(const std::string& filePath)
{
    return (AudioTrack*)GetResource(GetResourceFilePath(filePath));
}

Animation* ResourcesManager::GetAnimation(ResourceID animationID)
{
    return (Animation*)(GetResource(ResourceTypes::Animation, animationID));
}

Animation* ResourcesManager::GetAnimation(const std::string& filePath)
{
    return (Animation*)GetResource(GetResourceFilePath(filePath));
}

Font* ResourcesManager::LoadFont(const std::filesystem::path& filePath)
{
    if (!std::ifstream(filePath).good())
    {
        Log::LogError("Error loading font: file {0} does not exist", PathToString(filePath));
        return nullptr;
    }

    if (!FontManager::IsInitialized())
    {
        Log::LogError("Font manager is not initialized");
        return nullptr;
    }

    return FontManager::FontFromPath(PathToString(filePath).c_str());
}

Font* ResourcesManager::GetFont(ResourceID fontID)
{
    return (Font*)(GetResource(ResourceTypes::Font, fontID));
}

Font* ResourcesManager::GetFont(const std::string& filePath)
{
    return (Font*)GetResource(GetResourceFilePath(filePath));
}

Font* ResourcesManager::DefaultFont()
{
    return defaultFont;
}

Shader* ResourcesManager::LoadShader(const std::filesystem::path& fileVSPath, const std::filesystem::path& fileFSPath)
{
    if (!std::ifstream(fileVSPath).good() || !std::ifstream(fileFSPath).good())
    {
        Log::LogError("Error loading shader: file {0} or {1} does not exist", PathToString(fileVSPath), PathToString(fileFSPath));
        return nullptr;
    }

    return Shader::FromFilePaths(PathToString(fileVSPath).c_str(), PathToString(fileFSPath).c_str());
}

Shader* ResourcesManager::GetShader(ResourceID shaderID)
{
    return (Shader*)(GetResource(ResourceTypes::Shader, shaderID));
}

Shader* ResourcesManager::GetShader(const std::string& fileVSPath, const std::string& fileFSPath)
{
    // TODO: solve after combining shaders
    return (Shader*)GetResource(GetResourceFilePath(fileVSPath));
}

SceneData* ResourcesManager::LoadSceneData(const std::filesystem::path& filePath)
{
    if (!std::ifstream(filePath).good())
    {
        Log::LogError("Error loading scene data: file {0} does not exist", PathToString(filePath));
        return nullptr;
    }

    std::ifstream infile(filePath);
    YAML::Node node = YAML::Load(infile);
    auto sceneName = node["name"].as<std::string>();

    return new SceneData(sceneName);
}

SceneData* ResourcesManager::GetSceneData(ResourceID resourceID)
{
    return (SceneData*)(GetResource(ResourceTypes::SceneData, resourceID));
}

SceneData* ResourcesManager::GetSceneData(const std::string& filePath)
{
    return (SceneData*)GetResource(GetResourceFilePath(filePath));
}

int ResourcesManager::GetDefaultPixelsPerUnit()
{
    return defaultPixelsPerUnit;
}

void ResourcesManager::SetDefaultPixelsPerUnit(int newValue)
{
    defaultPixelsPerUnit = newValue < 1 ? 1 : newValue;
}

Shader* ResourcesManager::DefaultSpriteShader()
{
    return defaultSpriteShader;
}

Shader* ResourcesManager::DefaultUIShader()
{
    return defaultUIShader;
}

Material* ResourcesManager::GetMaterial(ResourceID materialID)
{
    return (Material*)(GetResource(ResourceTypes::Material, materialID));
}

Material* ResourcesManager::DefaultSpriteMaterial()
{
    return defaultSpriteMaterial;
}

Material* ResourcesManager::DefaultMeshMaterial()
{
    return defaultMeshMaterial;
}

Material* ResourcesManager::DefaultUIMaterial()
{
    return defaultUIMaterial;
}

Material* ResourcesManager::DefaultUIClippingMaterial()
{
    return defaultUIClippingMaterial;
}
