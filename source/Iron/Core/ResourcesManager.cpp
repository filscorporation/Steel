#include "ResourcesManager.h"
#include "../Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GLAD/glad.h>

Image* ResourcesManager::LoadImage(const char *filePath)
{
    //TODO: check if exists
    //TODO: completely rework

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

    auto image = new Image();
    image->ID = images.size();
    image->TextureID = (unsigned int)texture;
    image->Path = filePath;
    image->Width = w;
    image->Height = h;

    images.push_back(image);

    Log::LogInfo("Image loaded");

    return image;
}

Image *ResourcesManager::GetImage(unsigned int imageID)
{
    if (imageID > images.size() || images[imageID] == nullptr)
    {
        Log::LogError("Image does not exist");
        return nullptr;
    }

    return images[imageID];
}
