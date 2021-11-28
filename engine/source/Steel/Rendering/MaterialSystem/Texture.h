#pragma once

#include <cstdint>

#include "Steel/Resources/ResourceID.h"

class Texture
{
public:
    explicit Texture();
    ~Texture();

    ResourceID ID;

    static Texture* CreateImageTexture(unsigned char* imageData, uint32_t width, uint32_t height);
    static Texture* CreateColorAttachment(uint32_t width, uint32_t height);
    static Texture* CreateDSAttachment(uint32_t width, uint32_t height);

    uint32_t GetTextureID() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    uint32_t _textureID = 0;
    uint32_t _width = 0;
    uint32_t _height = 0;
};
