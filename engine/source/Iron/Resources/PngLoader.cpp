#include "PngLoader.h"
#include "ResourcesManager.h"
#include "../Rendering/OpenGLAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Sprite* PngLoader::LoadImage(const char* filePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, 4);
    uint32_t texture = OpenGLAPI::GenerateTexture(imageData, width, height);

    auto image = new Sprite();
    image->TextureID = texture;
    image->Path = filePath;
    image->Width = (uint32_t)width;
    image->Height = (uint32_t)height;
    image->IsTransparent = channels == 4 && ResourcesManager::IsImageTransparent(imageData, width, height);

    stbi_image_free(imageData);

    return image;
}
