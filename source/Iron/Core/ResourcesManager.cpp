#include "ResourcesManager.h"
#include "../Core/Log.h"
#include "../Audio/AudioSystem.h"
#include "../Audio/WavLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GLAD/glad.h>
#include <fstream>
// TODO: remove al dependency into AudioSystem
#include <AL/al.h>

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
}

Sprite* ResourcesManager::LoadImage(const char *filePath)
{
    //TODO: completely rework

    std::ifstream infile(filePath);
    if (!infile.good())
    {
        Log::LogError("File does not exist");
        return nullptr;
    }

    int w, h, c;
    unsigned char *imageData = stbi_load(filePath, &w, &h, &c, 4);
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint filtering = GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto image = new Sprite();
    image->ID = images.size();
    image->TextureID = (unsigned int)texture;
    image->Path = filePath;
    image->Width = w;
    image->Height = h;

    images.push_back(image);

    Log::LogInfo("Sprite loaded");

    return image;
}

Sprite* ResourcesManager::GetImage(unsigned int imageID)
{
    if (imageID > images.size() || images[imageID] == nullptr)
    {
        Log::LogError("Sprite does not exist");
        return nullptr;
    }

    return images[imageID];
}

void ResourcesManager::UnloadImage(unsigned int imageID)
{
    auto sprite = GetImage(imageID);
    if (sprite == nullptr)
        return;

    glDeleteTextures(1, &sprite->TextureID);
    //images.erase(images.begin() + imageID);
    images[imageID] = nullptr;
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

AudioTrack* ResourcesManager::LoadAudioTrack(const char *filePath)
{
    std::ifstream infile(filePath);
    if (!infile.good())
    {
        Log::LogError("File does not exist");
        return nullptr;
    }

    if (!AudioSystem::Initialized())
    {
        Log::LogError("Can't load audio: audio system is not initialized");
        return nullptr;
    }

    ALuint audioBuffer;
    char* data;
    auto audioTrack = WavLoader::LoadWav(filePath, &data);
    if (audioTrack == nullptr)
        return nullptr;

    audioTrack->ID = audioTracks.size();

    alGenBuffers((ALuint)1, &audioBuffer);
    if (AudioSystem::CheckForErrors())
    {
        Log::LogError("Error generating audio buffer");
        return nullptr;
    }
    alBufferData(audioBuffer, ToALFormat(audioTrack->NumberOfChannels, audioTrack->BitsPerSample),
                 data, (ALsizei)audioTrack->NumberOfSamples, audioTrack->SampleRate);
    delete data;
    if (AudioSystem::CheckForErrors())
    {
        Log::LogError("Error loading audio data to buffer");
        delete audioTrack;
        return nullptr;
    }

    audioTrack->BufferID = audioBuffer;
    audioTracks.push_back(audioTrack);

    Log::LogInfo("Audio track loaded");

    return audioTrack;
}

AudioTrack *ResourcesManager::GetAudioTrack(unsigned int audioID)
{
    if (audioID > images.size() || images[audioID] == nullptr)
    {
        Log::LogError("Audio track does not exist");
        return nullptr;
    }

    return audioTracks[audioID];
}

void ResourcesManager::UnloadAudioTrack(unsigned int audioID)
{
    auto audioTrack = GetAudioTrack(audioID);
    if (audioTrack == nullptr)
        return;

    alDeleteBuffers(1, &audioTrack->BufferID);
    //audioTracks.erase(audioTracks.begin() + audioID);
    audioTracks[audioID] = nullptr;
    delete audioTrack;

}

void ResourcesManager::AddAnimation(Animation *animation)
{
    animation->ID = animations.size();
    animations.push_back(animation);
}

Animation *ResourcesManager::GetAnimation(unsigned int animationID)
{
    if (animationID > animations.size() || animations[animationID] == nullptr)
    {
        Log::LogError("Animation does not exist");
        return nullptr;
    }

    return animations[animationID];
}

void ResourcesManager::RemoveAnimation(unsigned int animationID)
{
    auto animation = GetAnimation(animationID);
    if (animation == nullptr)
        return;

    //animations.erase(animations.begin() + animationID);
    animations[animationID] = nullptr;
    delete animation;
}
