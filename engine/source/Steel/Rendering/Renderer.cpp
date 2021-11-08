#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Screen.h"
#include "OpenGLAPI.h"
#include "../Core/Application.h"
#include "../Core/Log.h"

#define VIEW_PROJECTION "view_projection"

const int RENDER_CALL_DATA_SIZE = 9;
const int MAX_RENDER_CALLS = 10000;

int renderCallsCount;
float* vertexBufferData;
uint32_t indexBufferID, vertexDataBufferID;

glm::mat4 Renderer::currentViewProjection;
size_t Renderer::lastPropertyBlockHash = 0;
ResourceID Renderer::lastMaterial = NULL_RESOURCE;
std::vector<Shader*> Renderer::shadersUsed;
bool Renderer::drawWireframe = false;;
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

using namespace OpenGLAPI;

void Renderer::Init()
{
    Log::LogDebug("Begin renderer init");

    if (!OpenGLAPI::Init())
    {
        Log::LogError("Error loading OpenGL");
        return;
    }

    EnableDepthTest();
    EnableStencilTest();
    EnableBlend();

    Log::LogDebug("Uniforms saved");

    // Reserve space for all vertex data
    vertexBufferData = new float[MAX_RENDER_CALLS * RENDER_CALL_DATA_SIZE * 4];
    vertexDataBufferID = GenerateVertexBuffer(MAX_RENDER_CALLS * RENDER_CALL_DATA_SIZE * 4, nullptr, Dynamic);

    Log::LogDebug("Vertices data generated");

    // Generate all vertex indices
    auto indexes = new uint32_t[MAX_RENDER_CALLS * 6];
    int indexOffset = 0;
    for (int i = 0; i < MAX_RENDER_CALLS * 6; i += 6)
    {
        indexes[i + 0] = indexOffset + 0;
        indexes[i + 1] = indexOffset + 1;
        indexes[i + 2] = indexOffset + 2;

        indexes[i + 3] = indexOffset + 1;
        indexes[i + 4] = indexOffset + 2;
        indexes[i + 5] = indexOffset + 3;

        indexOffset += 4;
    }
    indexBufferID = GenerateIndexBuffer(MAX_RENDER_CALLS * 6, indexes, Static);
    delete[] indexes;

    Log::LogDebug("Indexes generated");

    wireframeShader = new Shader(WireframeVS, WireframeFS);
    wireframeMaterial = new Material();
    wireframeMaterial->MainShader = wireframeShader;

    Log::LogDebug("Renderer initialized");
}

void Renderer::Terminate()
{
    delete wireframeShader;

    DeleteBuffer(indexBufferID);
    DeleteBuffer(vertexDataBufferID);

    delete[] vertexBufferData;
}

void Renderer::OnBeforeRender(Camera& camera)
{
    // Set camera transformation
    currentViewProjection = camera.GetViewProjection();

    // Start first batch
    StartBatch();

    Application::Context()->Stats.DrawCalls = 0;
    Application::Context()->Stats.VerticesCount = 0;
}

void Renderer::OnAfterRender()
{
    EndBatch();

    for (auto& shader : shadersUsed)
        shader->GlobalUniformsSet = false;
    shadersUsed.clear();
    lastPropertyBlockHash = 0;
    lastMaterial = NULL_RESOURCE;
}

void Renderer::Clear(glm::vec3 color)
{
    SetClearColor(color.r, color.g, color.b, 1.0f);
    ClearColor();
    ClearDepth();
    ClearStencil();
}

void Renderer::PrepareUIRender()
{
    EndBatch();
    StartBatch();

    // Set camera transformation
    currentViewProjection = Screen::GetUIViewProjection();
    ClearDepth();
    ClearStencil();
}

void Renderer::Draw(const QuadRenderer& quad)
{
    if (!drawWireframe)
    {
        if (quad.RenderMaterial->ID != lastMaterial)
        {
            EndBatch();
            StartBatch();
            lastMaterial = quad.RenderMaterial->ID;
            lastPropertyBlockHash = 0;
            quad.RenderMaterial->MainShader->Use();
            quad.RenderMaterial->Properties.Apply(quad.RenderMaterial->MainShader);
            quad.CustomProperties.Apply(quad.RenderMaterial->MainShader);
        }
        else if (quad.CustomProperties.GetHash() != lastPropertyBlockHash)
        {
            EndBatch();
            StartBatch();
            lastPropertyBlockHash = quad.CustomProperties.GetHash();
            quad.CustomProperties.Apply(quad.RenderMaterial->MainShader);
        }

        if (!quad.RenderMaterial->MainShader->GlobalUniformsSet)
        {
            shadersUsed.push_back(quad.RenderMaterial->MainShader);
            quad.RenderMaterial->MainShader->GlobalUniformsSet = true;
            quad.RenderMaterial->Properties.SetMat4(VIEW_PROJECTION, glm::value_ptr(currentViewProjection));
        }
    }

    int offset = renderCallsCount * RENDER_CALL_DATA_SIZE * 4;
    for (int i = 0; i < 4; ++i)
    {
        vertexBufferData[offset++] = quad.Vertices[i][0];
        vertexBufferData[offset++] = quad.Vertices[i][1];
        vertexBufferData[offset++] = quad.Vertices[i][2];
        vertexBufferData[offset++] = quad.Color[0];
        vertexBufferData[offset++] = quad.Color[1];
        vertexBufferData[offset++] = quad.Color[2];
        vertexBufferData[offset++] = quad.Color[3];
        vertexBufferData[offset++] = quad.TextureCoords[i][0];
        vertexBufferData[offset++] = quad.TextureCoords[i][1];
    }

    renderCallsCount++;
    Application::Context()->Stats.VerticesCount += 4;

    if (renderCallsCount >= MAX_RENDER_CALLS)
    {
        EndBatch();
        StartBatch();

        lastMaterial = NULL_RESOURCE;
        lastPropertyBlockHash = 0;
    }
}

void Renderer::StartBatch()
{
    renderCallsCount = 0;
}

void Renderer::EndBatch()
{
    if (renderCallsCount == 0)
        return;

    Application::Context()->Stats.DrawCalls ++;
    DrawBatchedData();
}

void Renderer::DrawBatchedData()
{
    // Generate vertex array
    uint32_t vertexArrayID = GenerateVertexArray();
    BindVertexArray(vertexArrayID);

    // Set vertices data
    BindVertexBuffer(vertexDataBufferID);
    SetVertexBufferSubData(renderCallsCount * RENDER_CALL_DATA_SIZE * 4, vertexBufferData);

    // Bind indexes buffer and define attributes
    BindIndexBuffer(indexBufferID);

    // Vertex positions
    EnableVertexFloatAttribute(0, 3, RENDER_CALL_DATA_SIZE, 0);
    // Vertex color
    EnableVertexFloatAttribute(1, 4, RENDER_CALL_DATA_SIZE, 3);
    // Texture coords
    EnableVertexFloatAttribute(2, 2, RENDER_CALL_DATA_SIZE, 7);

    UnbindVertexBuffer();

    // Draw
    DrawTriangles(renderCallsCount * 6);

    UnbindVertexArray();

    UnbindTexture();
    DeleteVertexArray(vertexArrayID);
}

void Renderer::BeginWireframeMode()
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
}
