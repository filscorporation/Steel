#include "Renderer.h"
#include "Screen.h"
#include "Core/RenderContext.h"
#include "Core/RenderList.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "Steel/Rendering/Core/DrawCallsSource.h"

Shader* Renderer::wireframeShader = nullptr;
Material* Renderer::wireframeMaterial = nullptr;

const char* WireframeVS =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 view_projection;\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"}";

const char* WireframeFS =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}";

void Renderer::Init()
{
    Log::LogDebug("Begin renderer init");

    if (!OpenGLAPI::Init())
    {
        Log::LogError("Error loading OpenGL");
        return;
    }

    OpenGLAPI::EnableDepthTest();
    OpenGLAPI::EnableStencilTest();
    OpenGLAPI::EnableBlend();

    wireframeShader = new Shader(WireframeVS, WireframeFS);
    wireframeMaterial = new Material();
    wireframeMaterial->MainShader = wireframeShader;

    Log::LogDebug("Renderer initialized");
}

void Renderer::Terminate()
{
    delete wireframeShader;
}

/*void Renderer::BeginWireframeMode()
{
    DisableDepthTest();
    SetPolygonMode(Line);
    drawWireframe = true;

    wireframeShader->Use();
    wireframeMaterial->Properties.SetMat4(VIEW_PROJECTION, glm::value_ptr(currentViewProjection));
    wireframeMaterial->Properties.Apply(wireframeShader);
}

void Renderer::EndWireframeMode()
{
    drawWireframe = false;
    SetPolygonMode(Fill);
    EnableDepthTest();
}*/

void Renderer::Draw(RenderTask& renderTask)
{
    // Prepare
    renderTask.TargetFramebuffer->Bind();
    Clear(renderTask.ClearFlagsValue);

    // Collect and execute draw calls
    RenderContext renderContext;
    renderContext.Task = &renderTask;

    renderTask.Source->CollectDrawCalls(&renderContext);
    renderContext.List.SortDrawCalls(&renderContext);
    renderContext.List.ExecuteDrawCalls(&renderContext);

    // Clear
    renderTask.TargetFramebuffer->Unbind();
}

void Renderer::Clear(ClearFlags::ClearFlag clearFlag)
{
    // TODO: move from renderer
    if (clearFlag & ClearFlags::Color)
    {
        auto color = Screen::GetColor();
        OpenGLAPI::SetClearColor(color.r, color.g, color.b, 1.0f);
        OpenGLAPI::ClearColor();
    }
    if (clearFlag & ClearFlags::Depth)
        OpenGLAPI::ClearDepth();
    if (clearFlag & ClearFlags::Stencil)
        OpenGLAPI::ClearStencil();
}
