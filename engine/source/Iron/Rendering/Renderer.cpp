#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Screen.h"
#include "SpriteRenderer.h"
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
std::vector<Shader*> Renderer::shadersUsed;
DrawModes::DrawMode Renderer::currentDrawMode = DrawModes::Normal;
bool Renderer::DrawWireframe = false;
int Renderer::DrawCallsStats = 0;
int Renderer::VerticesStats = 0;

using namespace OpenGLAPI;

void Renderer::Init()
{
    Log::LogDebug("Begin renderer init");

    if(!OpenGLAPI::Init())
    {
        Log::LogError("Error loading OpenGL");
        return;
    }

    EnableDepthTest();
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

    Log::LogDebug("Renderer initialized");
}

void Renderer::Terminate()
{
    DeleteBuffer(indexBufferID);
    DeleteBuffer(vertexDataBufferID);

    delete[] vertexBufferData;
}

void Renderer::OnBeforeRender(Camera& camera)
{
    // Set camera transformation
    currentViewProjection = camera.GetViewProjection();

    SetDrawMode(DrawModes::Normal);

    // Start first batch
    StartBatch();

    DrawCallsStats = 0;
    VerticesStats = 0;
}

void Renderer::OnAfterRender()
{
    EndBatch();

    for (auto& shader : shadersUsed)
        shader->GlobalUniformsSet = false;
    shadersUsed.clear();
}

void Renderer::Clear(glm::vec3 color)
{
    SetClearColor(color.r, color.g, color.b, 1.0f);
    ClearColor();
    ClearDepth();
}

void Renderer::PrepareUIRender()
{
    EndBatch();
    StartBatch();

    // Set camera transformation
    currentViewProjection = Screen::GetUIViewProjection();
    ClearDepth();
}

void Renderer::Draw(const QuadRenderer& quad)
{
    EndBatch();
    StartBatch();

    // TODO: Try to batch
    quad.Material->MainShader->Use();
    quad.Material->Properties.Apply(quad.Material->MainShader);
    quad.CustomProperties.Apply(quad.Material->MainShader);

    if (!quad.Material->MainShader->GlobalUniformsSet)
    {
        shadersUsed.push_back(quad.Material->MainShader);
        quad.Material->MainShader->GlobalUniformsSet = true;
        quad.Material->Properties.SetMat4(VIEW_PROJECTION, glm::value_ptr(currentViewProjection));
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
    VerticesStats += 4;
}

void Renderer::SetDrawMode(DrawModes::DrawMode drawMode)
{
    // TODO: not implemented
    return;

    if (currentDrawMode == drawMode)
        return;

    currentDrawMode = drawMode;
    //SetUniformInteger(drawModeUniform, (int)currentDrawMode);
}

void Renderer::StartBatch()
{
    renderCallsCount = 0;
}

void Renderer::EndBatch()
{
    if (renderCallsCount == 0)
        return;

    DrawCallsStats ++;
    DrawBatchedData();

    if (DrawWireframe)
    {
        // Wireframe mode
        auto drawModeBackup = currentDrawMode;
        SetDrawMode(DrawModes::Wireframe);

        DisableDepthTest();
        SetPolygonMode(Line);

        DrawBatchedData();

        SetPolygonMode(Fill);
        EnableDepthTest();

        SetDrawMode(drawModeBackup);
    }
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
