#include "PngLoader.h"
#include "Steel/Core/Application.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Sprite* PngLoader::LoadImage(const FileData& fileData)
{
    int width, height, channels;
    
    unsigned char* imageData = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(fileData.Data), (int)fileData.Size, &width, &height, &channels, 4);
    Texture* texture = Texture::CreateImageTexture(imageData, (uint32_t)width, (uint32_t)height);
    Application::Context()->Resources->AddResource(texture);

    auto image = new Sprite(texture);
    image->SetIsTransparent(channels == 4 && IsImageTransparent(imageData, width, height));

    stbi_image_free(imageData);

    return image;
}
