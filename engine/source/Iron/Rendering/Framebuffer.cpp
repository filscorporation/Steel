#include "Framebuffer.h"
#include "OpenGLAPI.h"
#include "../Core/Log.h"

#define DEFAULT_FRAMEBUFER_ID 0

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
    framebufferID = OpenGLAPI::CreateFramebuffer();
    OpenGLAPI::BindFramebuffer(framebufferID);

    colorAttachment = Texture::CreateColorAttachment(width, height);
    OpenGLAPI::SetFramebufferColorAttachment(colorAttachment->GetTextureID());

    depthStencilAttachment = Texture::CreateDSAttachment(width, height);
    OpenGLAPI::SetFramebufferDSAttachment(depthStencilAttachment->GetTextureID());

    if (!OpenGLAPI::FramebufferComplete())
    {
        Log::LogError("Framebuffer status is not complete");
    }
    Unbind();
}

Framebuffer::Framebuffer()
{
    framebufferID = DEFAULT_FRAMEBUFER_ID;
}

Framebuffer::~Framebuffer()
{
    if (framebufferID != DEFAULT_FRAMEBUFER_ID)
        OpenGLAPI::DeleteFramebuffer(framebufferID);
    if (colorAttachment != nullptr)
    {
        OpenGLAPI::DeleteTexture(colorAttachment->GetTextureID());
        delete colorAttachment;
    }
    if (depthStencilAttachment != nullptr)
    {
        OpenGLAPI::DeleteTexture(depthStencilAttachment->GetTextureID());
        delete depthStencilAttachment;
    }
}

void Framebuffer::Bind() const
{
    OpenGLAPI::BindFramebuffer(framebufferID);
}

void Framebuffer::Unbind()
{
    OpenGLAPI::BindFramebuffer(DEFAULT_FRAMEBUFER_ID);
}

Texture* Framebuffer::GetColorAttachment() const
{
    return colorAttachment;
}
