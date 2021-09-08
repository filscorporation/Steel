#include "PngLoader.h"
#include "../Core/Application.h"
#include "../Rendering/OpenGLAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Sprite* PngLoader::LoadImage(const char* filePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, 4);
    Texture* texture = Texture::CreateImageTexture(imageData, (uint32_t)width, (uint32_t)height);
    Application::Context()->Resources->AddTexture(texture);

    auto image = new Sprite(texture);
    image->Path = filePath;
    image->IsTransparent = channels == 4 && IsImageTransparent(imageData, width, height);

    stbi_image_free(imageData);

    return image;
}
