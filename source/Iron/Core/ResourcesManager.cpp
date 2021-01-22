#include "ResourcesManager.h"
#include "../Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GLAD/glad.h>
#include <fstream>

ResourcesManager::~ResourcesManager()
{
    for (auto image : images)
    {
        glDeleteTextures(1, &image->TextureID);
        delete image;
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

    glGenTextures(2, &texture);
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
    images.erase(images.begin() + imageID);
    delete sprite;
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

    images.erase(images.begin() + animationID);
    delete animation;
}
