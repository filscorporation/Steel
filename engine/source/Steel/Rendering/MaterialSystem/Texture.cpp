#include "Texture.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"

Texture::Texture()
{
    Type = ResourceTypes::Texture;
}

Texture::~Texture()
{
    if (ID != NULL_RESOURCE)
        OpenGLAPI::DeleteTexture(_textureID);
}

Texture* Texture::CreateImageTexture(unsigned char *imageData, uint32_t width, uint32_t height)
{
    auto texture = new Texture();

    texture->_width = width;
    texture->_height = height;
    texture->_textureID = OpenGLAPI::GenerateTexture(imageData, width, height, OpenGLAPI::Nearest);

    return texture;
}

Texture* Texture::CreateColorAttachment(uint32_t width, uint32_t height)
{
    auto texture = new Texture();

    texture->_width = width;
    texture->_height = height;
    texture->_textureID = OpenGLAPI::GenerateColorAttachmentTexture(width, height);

    return texture;
}

Texture* Texture::CreateDSAttachment(uint32_t width, uint32_t height)
{
    auto texture = new Texture();

    texture->_width = width;
    texture->_height = height;
    texture->_textureID = OpenGLAPI::GenerateDSAttachmentTexture(width, height);

    return texture;
}

uint32_t Texture::GetTextureID() const
{
    return _textureID;
}

uint32_t Texture::GetWidth() const
{
    return _width;
}

uint32_t Texture::GetHeight() const
{
    return _height;
}
