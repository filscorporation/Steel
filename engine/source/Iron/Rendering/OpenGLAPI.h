#pragma once

#include <cstdint>

class OpenGLAPI
{
public:
    static uint32_t GenerateTexture(unsigned char* imageData, uint32_t width, uint32_t height);
    static uint32_t BeginGenerateMonochromeTexture(uint32_t width, uint32_t height);
    static void SetMonochromeTextureSubImage(unsigned char* imageData, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void EndGenerateTexture();
    static void DeleteTexture(uint32_t textureID);
};
