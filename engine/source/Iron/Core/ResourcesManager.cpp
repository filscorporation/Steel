#include "ResourcesManager.h"
#include "../Core/Log.h"
#include "../Audio/AudioCore.h"
#include "../Audio/WavLoader.h"
#include "../UI/FontManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GLAD/glad.h>
#include <fstream>
// TODO: remove al dependency into AudioCore
#include <AL/al.h>

#ifdef DISTRIBUTE_BUILD
#define ENGINE_RESOURCES_PATH "Resources/"
#define RESOURCES_PATH "Resources/"
#else
#define ENGINE_RESOURCES_PATH "../../../engine/resources/"
#define RESOURCES_PATH "../resources/"
#endif

ResourcesManager::ResourcesManager()
{
    FontManager::Init();
}

ResourcesManager::~ResourcesManager()
{
    for (auto image : images)
    {
        glDeleteTextures(1, &image->TextureID);
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
}

void ResourcesManager::LoadDefaultFont()
{
    defaultFont = LoadFont("font.ttf", true);
    if (defaultFont != nullptr)
        defaultFont->AddSizeIfNotExists(32);
}

Sprite* ResourcesManager::LoadImage(const char* filePath, bool engineResource)
{
    //TODO: completely rework

    std::string fullPathString = engineResource ? ENGINE_RESOURCES_PATH : RESOURCES_PATH;
    fullPathString += filePath;
    const char* fullPath = fullPathString.c_str();

    std::ifstream infile(fullPath);
    if (!infile.good())
    {
        Log::LogError("Error loading image: file does not exist");
        return nullptr;
    }

    int width, height, channels;
    unsigned char* imageData = stbi_load(fullPath, &width, &height, &channels, 4);
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto image = new Sprite();
    image->ID = images.size() + 1;
    image->TextureID = (uint64_t)texture;
    image->Path = fullPath;
    image->Width = width;
    image->Height = height;
    image->IsTransparent = channels == 4 && IsImageTransparent(imageData, width, height);

    stbi_image_free(imageData);

    images.push_back(image);

    Log::LogInfo("Sprite loaded: " + std::string(fullPath) + ", " + std::to_string(image->ID));

    return image;
}

Sprite* ResourcesManager::GetImage(ResourceID imageID)
{
    if (imageID == NULL_RESOURCE)
        return nullptr;

    if (imageID - 1 > images.size() || images[imageID - 1] == nullptr)
    {
        Log::LogError("Sprite does not exist: " + std::to_string(imageID));
        return nullptr;
    }

    return images[imageID - 1];
}

void ResourcesManager::UnloadImage(ResourceID imageID)
{
    auto sprite = GetImage(imageID);
    if (sprite == nullptr)
        return;

    glDeleteTextures(1, &sprite->TextureID);
    //images.erase(images.begin() + imageID); // TODO: remove
    images[imageID - 1] = nullptr;
    delete sprite;
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

    Log::LogInfo("Audio track loaded: " + std::string(fullPath) + ", " + std::to_string(audioTrack->ID));

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

    Log::LogInfo("Font loaded: " + std::string(fullPath) + ", " + std::to_string(font->ID));

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

Font *ResourcesManager::DefaultFont()
{
    return defaultFont;
}

bool ResourcesManager::IsImageTransparent(const unsigned char* imageData, int width, int height)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            auto alpha = imageData[(i * width + j) * 4 + 3];
            if (alpha != 0 && alpha != 255)
                return true;
        }
    }

    return false;
}
