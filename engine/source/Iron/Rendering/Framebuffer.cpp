#include "Framebuffer.h"
#include "OpenGLAPI.h"
#include "../Core/Application.h"
#include "../Core/Log.h"

#define DEFAULT_FRAMEBUFER_ID 0

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;

    framebufferID = OpenGLAPI::CreateFramebuffer();
    OpenGLAPI::BindFramebuffer(framebufferID);

    colorAttachment = Texture::CreateColorAttachment(width, height);
    Application::Context()->Resources->AddTexture(colorAttachment);
    OpenGLAPI::SetFramebufferColorAttachment(colorAttachment->GetTextureID());

    depthStencilAttachment = Texture::CreateDSAttachment(width, height);
    Application::Context()->Resources->AddTexture(depthStencilAttachment);
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
        Application::Context()->Resources->UnloadTexture(colorAttachment->ID);
    if (depthStencilAttachment != nullptr)
        Application::Context()->Resources->UnloadTexture(depthStencilAttachment->ID);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;

    if (colorAttachment != nullptr)
        Application::Context()->Resources->UnloadTexture(colorAttachment->ID);
    if (depthStencilAttachment != nullptr)
        Application::Context()->Resources->UnloadTexture(depthStencilAttachment->ID);

    OpenGLAPI::BindFramebuffer(framebufferID);

    colorAttachment = Texture::CreateColorAttachment(width, height);
    Application::Context()->Resources->AddTexture(colorAttachment);
    OpenGLAPI::SetFramebufferColorAttachment(colorAttachment->GetTextureID());

    depthStencilAttachment = Texture::CreateDSAttachment(width, height);
    Application::Context()->Resources->AddTexture(depthStencilAttachment);
    OpenGLAPI::SetFramebufferDSAttachment(depthStencilAttachment->GetTextureID());

    if (!OpenGLAPI::FramebufferComplete())
    {
        Log::LogError("Framebuffer status is not complete");
    }
    Unbind();
}

void Framebuffer::Bind() const
{
    OpenGLAPI::BindFramebuffer(framebufferID);
    if (framebufferID == DEFAULT_FRAMEBUFER_ID)
        OpenGLAPI::SetViewport(0, 0, Application::Context()->ScreenParams.Width, Application::Context()->ScreenParams.Height);
    else
        OpenGLAPI::SetViewport(0, 0, (int)_width, (int)_height);
}

void Framebuffer::Unbind()
{
    OpenGLAPI::BindFramebuffer(DEFAULT_FRAMEBUFER_ID);
}

Texture* Framebuffer::GetColorAttachment() const
{
    return colorAttachment;
}
