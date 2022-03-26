#include "ResourcesManager.h"
#include "AsepriteLoader.h"
#include "PngLoader.h"
#include "Steel/Core/Log.h"
#include "Steel/Math/Math.h"
#include "Steel/Math/Random.h"
#include "Steel/Audio/AudioCore.h"
#include "Steel/Audio/WavLoader.h"
#include "Steel/Rendering/MaterialSystem/BuiltInShaders.h"
#include "Steel/UI/FontManager.h"

#include <filesystem>

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

void ResourcesManager::LoadResources()
{
    if (!std::filesystem::exists(RESOURCES_PATH))
    {
        Log::LogWarning("Resources not found in path: \"{0}\"", RESOURCES_PATH);
        return;
    }

    for (std::filesystem::recursive_directory_iterator i(RESOURCES_PATH), end; i != end; ++i)
        if (!is_directory(i->path()))
            TryLoadResource(i->path().filename().u8string());
}

void ResourcesManager::TryLoadResource(const std::string& path)
{
    std::string extension = path.substr(path.find_last_of('.') + 1);
    if (extension == "png")
        LoadSprite(path.c_str());
    else if (extension == "wav")
        LoadAudioTrack(path.c_str());
    else if (extension == "ttf")
        LoadFont(path.c_str());
    else if (extension == "aseprite")
        LoadAsepriteData(path.c_str());
    else if (extension == "vs")
    {
        // TODO: combine shaders into one file
        std::string fsPath = path;
        fsPath.replace(path.length() - 2, 2, "fs");
        std::ifstream infile(fsPath);
        if (infile.good())
            // Load only if there is fragment shader file with the same name
            LoadAsepriteData(path.c_str());
    }
}

void ResourcesManager::LoadDefaultResources()
{
    defaultFont = LoadFont("font.ttf", true);
    if (defaultFont != nullptr)
        defaultFont->AddSizeIfNotExists(32);

    defaultSpriteShader = new Shader(BuiltInShaders::DefaultSpriteVS, BuiltInShaders::DefaultSpriteFS);
    defaultSpriteShader->Path = "@defaultSpriteShader";
    AddResource(defaultSpriteShader);

    defaultMeshShader = new Shader(BuiltInShaders::DefaultMeshVS, BuiltInShaders::DefaultMeshFS);
    defaultMeshShader->Path = "@defaultMeshShader";
    AddResource(defaultMeshShader);

    defaultUIShader = new Shader(BuiltInShaders::DefaultUIVS, BuiltInShaders::DefaultUIFS);
    defaultUIShader->Path = "@defaultUIShader";
    AddResource(defaultUIShader);

    defaultUIClippingShader = new Shader(BuiltInShaders::DefaultUIClippingVS, BuiltInShaders::DefaultUIClippingFS);
    defaultUIClippingShader->Path = "@defaultUIClippingShader";
    AddResource(defaultUIClippingShader);

    defaultSpriteMaterial = new Material();
    defaultSpriteMaterial->MainShader = defaultSpriteShader;
    defaultSpriteMaterial->Path = "@defaultSpriteMaterial";
    AddResource(defaultSpriteMaterial);

    defaultMeshMaterial = new Material();
    defaultMeshMaterial->MainShader = defaultMeshShader;
    defaultMeshMaterial->Path = "@defaultMeshMaterial";
    AddResource(defaultMeshMaterial);

    defaultUIMaterial = new Material();
    defaultUIMaterial->MainShader = defaultUIShader;
    defaultUIMaterial->Path = "@defaultUIMaterial";
    AddResource(defaultUIMaterial);

    defaultUIClippingMaterial = new Material();
    defaultUIClippingMaterial->MainShader = defaultUIClippingShader;
    defaultUIClippingMaterial->Path = "@defaultUIClippingMaterial";
    AddResource(defaultUIClippingMaterial);
}

const char* ResourcesManager::GetResourcesPath()
{
    return RESOURCES_PATH;
}

std::string ResourceTypeToString(ResourceTypes::ResourceType type)
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
    }

    return "";
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

    resource->ID = resource->Path.empty() ? Random::NextULong() : Math::StringHash(resource->Path.c_str());

    if (resources[(int)resource->Type].find(resource->ID) != resources[(int)resource->Type].end())
    {
        Log::LogWarning("Replacing existing resource {0} ({1})", resource->ID, ResourceTypeToString(resource->Type));
        UnloadResource(resource->Type, resource->ID);
    }

    resources[(int)resource->Type][resource->ID] = resource;
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
    delete resource;
}

Sprite* ResourcesManager::LoadSprite(const char* filePath, bool engineResource)
{
    std::string path = engineResource ? std::string(ENGINE_RESOURCES_PATH) + filePath : filePath;

    uint64_t hash = Math::StringHash(path.c_str());
    if (ResourceExists(ResourceTypes::Sprite, hash))
        return GetSprite(hash);

    std::string fullPathString = engineResource ? ENGINE_RESOURCES_PATH : RESOURCES_PATH;
    fullPathString += filePath;
    std::string fullPath = fullPathString;

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading image: file {0} does not exist", fullPath);
        return nullptr;
    }

    Sprite* image;
    std::string extension = fullPath.substr(fullPath.find_last_of('.') + 1);
    if (extension == "png")
    {
        image = PngLoader::LoadImage(fullPath.c_str(), path.c_str());
        if (image == nullptr)
            return nullptr;

        image->Path = path;
        AddResource(image);
    }
    else if (extension == "aseprite")
    {
        AsepriteData data;
        data.Path = path;
        if (!AsepriteLoader::LoadAsepriteData(fullPath.c_str(), false, data))
            return nullptr;

        if (data.Sprites.empty())
        {
            Log::LogError("Error loading image: no data in aseprite file");
            return nullptr;
        }
        if (data.Sprites.size() > 1)
            Log::LogWarning("Aseprite file contains multiple images but only first will be used");

        image = data.Sprites[0];
    }
    else
    {
        Log::LogError("Error loading image: .{0} files not supported", extension);
        return nullptr;
    }

    image->PixelsPerUnit = defaultPixelsPerUnit;

    Log::LogDebug("Sprite loaded: {0}, {1}", fullPath, image->ID);

    return image;
}

Sprite* ResourcesManager::GetSprite(ResourceID imageID)
{
    return (Sprite*)(GetResource(ResourceTypes::Sprite, imageID));
}

Texture* ResourcesManager::GetTexture(ResourceID textureID)
{
    return (Texture*)(GetResource(ResourceTypes::Texture, textureID));
}

AsepriteData* ResourcesManager::LoadAsepriteData(const char* filePath, bool loopAll)
{
    uint64_t hash = Math::StringHash(filePath);
    if (ResourceExists(ResourceTypes::AsepriteData, hash))
    {
        auto data = GetAsepriteData(hash);
        if (loopAll)
            for (auto animation : data->Animations)
                animation->Loop = true;

        return data;
    }

    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;
    std::string fullPath = fullPathString;
    auto data = new AsepriteData();
    data->Path = filePath;

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading aseprite file: file {0} does not exist", fullPath);
        delete data;
        return nullptr;
    }

    std::string extension = fullPath.substr(fullPath.find_last_of('.') + 1);
    if (extension != "aseprite")
    {
        Log::LogError("Error loading aseprite file: .{0} files not supported", extension);
        delete data;
        return nullptr;
    }

    if (!AsepriteLoader::LoadAsepriteData(fullPath.c_str(), loopAll, *data))
    {
        delete data;
        return nullptr;
    }

    AddResource(data);

    for (auto sprite : data->Sprites)
        sprite->PixelsPerUnit = defaultPixelsPerUnit;

    Log::LogDebug("Aseprite file loaded: {0}, {1}", fullPath, data->ID);

    return data;
}

AsepriteData* ResourcesManager::GetAsepriteData(ResourceID resourceID)
{
    return (AsepriteData*)(GetResource(ResourceTypes::AsepriteData, resourceID));
}

AudioTrack* ResourcesManager::LoadAudioTrack(const char* filePath)
{
    uint64_t hash = Math::StringHash(filePath);
    if (ResourceExists(ResourceTypes::AudioTrack, hash))
        return GetAudioTrack(hash);

    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;
    const char* fullPath = fullPathString.c_str();

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading audio track: file does not exist");
        return nullptr;
    }

    char* data;
    AudioTrack* audioTrack = WavLoader::LoadWav(fullPath, &data);
    if (audioTrack == nullptr)
        return nullptr;

    if (!AudioCore::InitAudioTrack(audioTrack, data))
    {
        delete[] data;
        delete audioTrack;
        return nullptr;
    }
    delete[] data;

    audioTrack->Path = filePath;
    AddResource(audioTrack);

    Log::LogDebug("Audio track loaded: {0}, {1}", fullPath, audioTrack->ID);

    return audioTrack;
}

AudioTrack* ResourcesManager::GetAudioTrack(ResourceID audioID)
{
    return (AudioTrack*)(GetResource(ResourceTypes::AudioTrack, audioID));
}

Animation* ResourcesManager::GetAnimation(ResourceID animationID)
{
    return (Animation*)(GetResource(ResourceTypes::Animation, animationID));
}

Font* ResourcesManager::LoadFont(const char* fontPath, bool engineResource)
{
    std::string path = engineResource ? std::string(ENGINE_RESOURCES_PATH) + fontPath : fontPath;

    uint64_t hash = Math::StringHash(path.c_str());
    if (ResourceExists(ResourceTypes::Font, hash))
        return GetFont(hash);

    std::string fullPathString = engineResource ? ENGINE_RESOURCES_PATH : RESOURCES_PATH;
    fullPathString += fontPath;
    const char* fullPath = fullPathString.c_str();

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading font: file does not exist");
        return nullptr;
    }

    if (!FontManager::IsInitialized())
    {
        Log::LogError("Font manager is not initialized");
        return nullptr;
    }

    Font* font = FontManager::FontFromPath(fullPath);

    if (font == nullptr)
        return nullptr;

    font->Path = path;
    AddResource(font);

    Log::LogDebug("Font loaded: {0}, {1}", fullPath, font->ID);

    return font;
}

Font* ResourcesManager::GetFont(ResourceID fontID)
{
    return (Font*)(GetResource(ResourceTypes::Font, fontID));
}

Font* ResourcesManager::DefaultFont()
{
    return defaultFont;
}

Shader* ResourcesManager::LoadShader(const char* fileVSPath, const char* fileFSPath)
{
    std::string path = std::string(fileVSPath) + "@" + fileFSPath;

    uint64_t hash = Math::StringHash(path.c_str());
    if (ResourceExists(ResourceTypes::Shader, hash))
        return GetShader(hash);

    std::string fullVSPathString = RESOURCES_PATH;
    fullVSPathString += fileVSPath;
    const char* fullVSPath = fullVSPathString.c_str();
    std::string fullFSPathString = RESOURCES_PATH;
    fullFSPathString += fileFSPath;
    const char* fullFSPath = fullFSPathString.c_str();

    Shader* shader = Shader::FromFilePaths(fullVSPath, fullFSPath);
    if (shader == nullptr)
        return nullptr;

    shader->Path = path;
    AddResource(shader);

    return shader;
}

Shader* ResourcesManager::GetShader(ResourceID shaderID)
{
    return (Shader*)(GetResource(ResourceTypes::Shader, shaderID));
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
