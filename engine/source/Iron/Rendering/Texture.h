#pragma once

#include <cstdint>

class Texture
{
public:
    static Texture* CreateImageTexture(unsigned char* imageData, uint32_t width, uint32_t height);
    static Texture* CreateColorAttachment(uint32_t width, uint32_t height);
    static Texture* CreateDSAttachment(uint32_t width, uint32_t height);

    uint32_t GetTextureID() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    uint32_t _textureID;
    uint32_t _width = 0;
    uint32_t _height = 0;
};
