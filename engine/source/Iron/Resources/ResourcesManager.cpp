#include "ResourcesManager.h"
#include "AsepriteLoader.h"
#include "PngLoader.h"
#include "../Core/Log.h"
#include "../Audio/AudioCore.h"
#include "../Audio/WavLoader.h"
#include "../Rendering/OpenGLAPI.h"
#include "../UI/FontManager.h"

#include <fstream>
// TODO: remove al dependency into AudioCore
#include <AL/al.h>

#ifdef DISTRIBUTE_BUILD
#define ENGINE_RESOURCES_PATH "Resources/"
#define RESOURCES_PATH "Resources/"
#else
#define ENGINE_RESOURCES_PATH "../../../engine/resources/"
#define RESOURCES_PATH "../../resources/"
#endif

ResourcesManager::ResourcesManager()
{
    FontManager::Init();
}

ResourcesManager::~ResourcesManager()
{
    for (auto image : images)
    {
        OpenGLAPI::DeleteTexture(image->TextureID);
        delete image;
    }

    for (auto audioTrack : audioTracks)
    {
        alDeleteBuffers(1, &audioTrack->BufferID);
        delete audioTrack;
    }

    for (auto animation : animations)
    {
        delete animation;
    }

    for (auto font : fonts)
    {
        delete font;
    }

    for (auto data : asepriteDatas)
    {
        delete data;
    }
}

void ResourcesManager::LoadDefaultFont()
{
    defaultFont = LoadFont("font.ttf", true);
    if (defaultFont != nullptr)
        defaultFont->AddSizeIfNotExists(32);
}

const char* ResourcesManager::GetResourcesPath()
{
    return RESOURCES_PATH;
}

Sprite* ResourcesManager::LoadImage(const char* filePath, bool engineResource)
{
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
        image = PngLoader::LoadImage(fullPath.c_str());
        if (image == nullptr)
            return nullptr;

        AddImage(image);
    }
    else if (extension == "aseprite")
    {
        AsepriteData data;
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

    Log::LogDebug("Sprite loaded: {0}, {1}", fullPath, image->ID);

    return image;
}

void ResourcesManager::AddImage(Sprite* image)
{
    image->ID = images.size() + 1;
    images.push_back(image);
}

Sprite* ResourcesManager::GetImage(ResourceID imageID)
{
    if (imageID == NULL_RESOURCE)
        return nullptr;

    if (imageID - 1 > images.size() || images[imageID - 1] == nullptr)
    {
        Log::LogError("Sprite does not exist: {0}", imageID);
        return nullptr;
    }

    return images[imageID - 1];
}

void ResourcesManager::UnloadImage(ResourceID imageID)
{
    auto sprite = GetImage(imageID);
    if (sprite == nullptr)
        return;

    OpenGLAPI::DeleteTexture(sprite->TextureID);
    //images.erase(images.begin() + imageID); // TODO: remove
    images[imageID - 1] = nullptr;
    delete sprite;
}

AsepriteData* ResourcesManager::LoadAsepriteData(const char* filePath, bool loopAll)
{
    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;
    std::string fullPath = fullPathString;
    auto data = new AsepriteData();

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading aseprite file: file {0} does not exist", fullPath);
        return nullptr;
    }

    std::string extension = fullPath.substr(fullPath.find_last_of('.') + 1);
    if (extension != "aseprite")
    {
        Log::LogError("Error loading aseprite file: .{0} files not supported", extension);
        return nullptr;
    }

    if (!AsepriteLoader::LoadAsepriteData(fullPath.c_str(), loopAll, *data))
    {
        delete data;
        return nullptr;
    }

    data->ID = asepriteDatas.size() + 1;
    asepriteDatas.push_back(data);

    Log::LogDebug("Aseprite file loaded: {0}, {1}", fullPath, data->ID);

    return data;
}

AsepriteData* ResourcesManager::GetAsepriteData(ResourceID resourceID)
{
    if (resourceID == NULL_RESOURCE)
        return nullptr;

    if (resourceID - 1 > asepriteDatas.size() || asepriteDatas[resourceID - 1] == nullptr)
    {
        Log::LogError("Aseprite data does not exist: {0}", resourceID);
        return nullptr;
    }

    return asepriteDatas[resourceID - 1];
}

static inline ALenum ToALFormat(int channels, int samples)
{
    bool stereo = (channels > 1);

    switch (samples) {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
    }
}

AudioTrack* ResourcesManager::LoadAudioTrack(const char* filePath)
{
    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;
    const char* fullPath = fullPathString.c_str();

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading audio track: file does not exist");
        return nullptr;
    }

    if (!AudioCore::Initialized())
    {
        Log::LogError("Can't load audio: audio system is not initialized");
        return nullptr;
    }

    ALuint audioBuffer;
    char* data;
    auto audioTrack = WavLoader::LoadWav(fullPath, &data);
    if (audioTrack == nullptr)
        return nullptr;

    audioTrack->ID = audioTracks.size() + 1;

    alGenBuffers((ALuint)1, &audioBuffer);
    if (AudioCore::CheckForErrors())
    {
        Log::LogError("Error generating audio buffer");
        return nullptr;
    }
    alBufferData(audioBuffer, ToALFormat(audioTrack->NumberOfChannels, audioTrack->BitsPerSample),
                 data, (ALsizei)audioTrack->NumberOfSamples, audioTrack->SampleRate);
    delete data;
    if (AudioCore::CheckForErrors())
    {
        Log::LogError("Error loading audio data to buffer");
        delete audioTrack;
        return nullptr;
    }

    audioTrack->BufferID = audioBuffer;
    audioTracks.push_back(audioTrack);

    Log::LogDebug("Audio track loaded: {0}, {1}", fullPath, audioTrack->ID);

    return audioTrack;
}

AudioTrack* ResourcesManager::GetAudioTrack(ResourceID audioID)
{
    if (audioID == NULL_RESOURCE)
        return nullptr;

    if (audioID - 1 > images.size() || images[audioID - 1] == nullptr)
    {
        Log::LogError("Audio track does not exist");
        return nullptr;
    }

    return audioTracks[audioID - 1];
}

void ResourcesManager::UnloadAudioTrack(ResourceID audioID)
{
    auto audioTrack = GetAudioTrack(audioID);
    if (audioTrack == nullptr)
        return;

    alDeleteBuffers(1, &audioTrack->BufferID);
    //audioTracks.erase(audioTracks.begin() + audioID); // TODO: remove
    audioTracks[audioID - 1] = nullptr;
    delete audioTrack;

}

void ResourcesManager::AddAnimation(Animation* animation)
{
    animation->ID = animations.size() + 1;
    if (animation->Name.empty())
    {
        animation->Name = std::to_string(animation->ID);
    }
    animations.push_back(animation);
}

Animation* ResourcesManager::GetAnimation(ResourceID animationID)
{
    if (animationID == NULL_RESOURCE)
        return nullptr;

    if (animationID - 1 > animations.size() || animations[animationID - 1] == nullptr)
    {
        Log::LogError("Animation does not exist");
        return nullptr;
    }

    return animations[animationID - 1];
}

void ResourcesManager::RemoveAnimation(ResourceID animationID)
{
    auto animation = GetAnimation(animationID);
    if (animation == nullptr)
        return;

    //animations.erase(animations.begin() + animationID); // TODO: remove
    animations[animationID - 1] = nullptr;
    delete animation;
}

Font* ResourcesManager::LoadFont(const char* fontPath, bool engineResource)
{
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

    auto font = FontManager::FontFromPath(fullPath);

    if (font == nullptr)
        return nullptr;

    font->ID = fonts.size() + 1;
    fonts.push_back(font);

    Log::LogDebug("Font loaded: {0}, {1}", fullPath, font->ID);

    return font;
}

Font* ResourcesManager::GetFont(ResourceID fontID)
{
    if (fontID == NULL_RESOURCE)
        return nullptr;

    if (fontID - 1 > fonts.size() || fonts[fontID - 1] == nullptr)
    {
        Log::LogError("Font does not exist");
        return nullptr;
    }

    return fonts[fontID - 1];
}

Font* ResourcesManager::DefaultFont()
{
    return defaultFont;
}
