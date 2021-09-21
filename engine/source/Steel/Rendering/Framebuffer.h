#pragma once

#include <cstdint>

#include "Texture.h"

class Framebuffer
{
public:
    Framebuffer(uint32_t width, uint32_t height);
    Framebuffer();
    ~Framebuffer();

    void Resize(uint32_t width, uint32_t height);

    void Bind() const;
    void Unbind();

    Texture* GetColorAttachment() const;

private:

    uint32_t _width, _height;

    uint32_t framebufferID;
    Texture* colorAttachment = nullptr;
    Texture* depthStencilAttachment = nullptr;
};
