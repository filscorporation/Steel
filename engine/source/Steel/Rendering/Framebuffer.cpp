#include "Framebuffer.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "../Core/Application.h"
#include "../Core/Log.h"

#define DEFAULT_FRAMEBUFFER_ID 0

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;

    framebufferID = OpenGLAPI::CreateFramebuffer();
    OpenGLAPI::BindFramebuffer(framebufferID);

    colorAttachment = Texture::CreateColorAttachment(width, height);
    Application::Context()->Resources->AddResource(colorAttachment);
    OpenGLAPI::SetFramebufferColorAttachment(colorAttachment->GetTextureID());

    depthStencilAttachment = Texture::CreateDSAttachment(width, height);
    Application::Context()->Resources->AddResource(depthStencilAttachment);
    OpenGLAPI::SetFramebufferDSAttachment(depthStencilAttachment->GetTextureID());

    if (!OpenGLAPI::FramebufferComplete())
    {
        Log::LogError("Framebuffer status is not complete");
    }
    Unbind();
}

Framebuffer::Framebuffer()
{
    framebufferID = DEFAULT_FRAMEBUFFER_ID;
}

Framebuffer::~Framebuffer()
{
    if (framebufferID != DEFAULT_FRAMEBUFFER_ID)
        OpenGLAPI::DeleteFramebuffer(framebufferID);
    if (colorAttachment != nullptr)
        Application::Context()->Resources->UnloadResource(ResourceTypes::Texture, colorAttachment->ID);
    if (depthStencilAttachment != nullptr)
        Application::Context()->Resources->UnloadResource(ResourceTypes::Texture, depthStencilAttachment->ID);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;

    if (colorAttachment != nullptr)
        Application::Context()->Resources->UnloadResource(ResourceTypes::Texture, colorAttachment->ID);
    if (depthStencilAttachment != nullptr)
        Application::Context()->Resources->UnloadResource(ResourceTypes::Texture, depthStencilAttachment->ID);

    OpenGLAPI::BindFramebuffer(framebufferID);

    colorAttachment = Texture::CreateColorAttachment(width, height);
    Application::Context()->Resources->AddResource(colorAttachment);
    OpenGLAPI::SetFramebufferColorAttachment(colorAttachment->GetTextureID());

    depthStencilAttachment = Texture::CreateDSAttachment(width, height);
    Application::Context()->Resources->AddResource(depthStencilAttachment);
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
    if (framebufferID == DEFAULT_FRAMEBUFFER_ID)
        OpenGLAPI::SetViewport(0, 0, Application::Context()->ScreenParams.Width, Application::Context()->ScreenParams.Height);
    else
        OpenGLAPI::SetViewport(0, 0, (int)_width, (int)_height);
}

void Framebuffer::Unbind()
{
    OpenGLAPI::BindFramebuffer(DEFAULT_FRAMEBUFFER_ID);
}

Texture* Framebuffer::GetColorAttachment() const
{
    return colorAttachment;
}
