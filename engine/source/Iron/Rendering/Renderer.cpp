#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "BuiltInShaders.h"
#include "Screen.h"
#include "SpriteRenderer.h"
#include "OpenGLAPI.h"
#include "../Core/Application.h"

const int RENDER_CALL_DATA_SIZE = 10;
const int MAX_RENDER_CALLS = 10000;
const int MAX_TEXTURE_SLOTS = 32;

Shader* shader;

int renderCallsCount;
int texturesCount;
float* vertexBufferData;
uint32_t textureIDs[MAX_TEXTURE_SLOTS];
uint32_t indexBufferID, vertexDataBufferID;
int viewProjectionUniform;
int drawModeUniform;

DrawModes::DrawMode Renderer::currentDrawMode = DrawModes::Normal;
bool Renderer::DrawWireframe = false;
int Renderer::DrawCallsStats = 0;
int Renderer::VerticesStats = 0;

using namespace OpenGLAPI;

void Renderer::Init()
{
    Log::LogInfo("Begin renderer init");

    if(!OpenGLAPI::Init())
    {
        Log::LogError("Error loading OpenGL");
        return;
    }

    EnableDepthTest();
    EnableBlend();

    // Create shader and initialize it's texture slots variable
    shader = new Shader(BuiltInShaders::VertexShader, BuiltInShaders::FragmentShader);
    shader->Use();
    int textureSlots[MAX_TEXTURE_SLOTS];
    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
        textureSlots[i] = i;
    SetUniformIntegers(GetUniformLocation(shader->Program, "images"), MAX_TEXTURE_SLOTS, textureSlots);

    Log::LogInfo("Shader created");

    // Camera transformation uniform
    viewProjectionUniform = GetUniformLocation(shader->Program, "view_projection");
    // Draw mode
    drawModeUniform = GetUniformLocation(shader->Program, "draw_mode");

    Log::LogInfo("Uniforms saved");

    // Reserve space for all vertex data
    vertexBufferData = new float[MAX_RENDER_CALLS * RENDER_CALL_DATA_SIZE * 4];
    vertexDataBufferID = GenerateVetexBuffer(MAX_RENDER_CALLS * RENDER_CALL_DATA_SIZE * 4, nullptr, Dynamic);

    Log::LogInfo("Vertices data generated");

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

    Log::LogInfo("Indexes generated");

    Log::LogInfo("Renderer initialized");
}

void Renderer::Terminate()
{
    DeleteBuffer(indexBufferID);
    DeleteBuffer(vertexDataBufferID);

    delete[] vertexBufferData;
    delete shader;
}

void Renderer::OnBeforeRender(Camera& camera)
{
    // Set camera transformation
    glm::mat4 viewProjection = camera.GetViewProjection();
    SetUniformMatrix4Float(viewProjectionUniform, glm::value_ptr(viewProjection));

    SetDrawMode(DrawModes::Normal);

    // Start first batch
    StartBatch();

    DrawCallsStats = 0;
    VerticesStats = 0;
}

void Renderer::OnAfterRender()
{
    EndBatch();
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

    glm::mat4 uiViewProjection = Screen::GetUIViewProjection();
    SetUniformMatrix4Float(viewProjectionUniform, glm::value_ptr(uiViewProjection));
    ClearDepth();
}

void Renderer::DrawQuad(glm::vec3 vertices[4], glm::vec2 textureCoords[4], const glm::vec4& color, uint32_t textureID)
{
    int textureIDIndex = FindTextureSlot(textureID);

    int offset = renderCallsCount * RENDER_CALL_DATA_SIZE * 4;
    for (int i = 0; i < 4; ++i)
    {
        vertexBufferData[offset++] = vertices[i][0];
        vertexBufferData[offset++] = vertices[i][1];
        vertexBufferData[offset++] = vertices[i][2];
        vertexBufferData[offset++] = color[0];
        vertexBufferData[offset++] = color[1];
        vertexBufferData[offset++] = color[2];
        vertexBufferData[offset++] = color[3];
        vertexBufferData[offset++] = textureCoords[i][0];
        vertexBufferData[offset++] = textureCoords[i][1];
        vertexBufferData[offset++] = (float)textureIDIndex;
    }

    renderCallsCount++;
    VerticesStats += 4;
}

void Renderer::SetDrawMode(DrawModes::DrawMode drawMode)
{
    if (currentDrawMode == drawMode)
        return;

    currentDrawMode = drawMode;
    SetUniformInteger(drawModeUniform, (int)currentDrawMode);
}

void Renderer::StartBatch()
{
    renderCallsCount = 0;
    texturesCount = 0;
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
    uint32_t vertexArrayID = GenerateVetexArray();
    BindVetexArray(vertexArrayID);

    // Set vertices data
    BindVetexBuffer(vertexDataBufferID);
    SetVetexBufferSubData(renderCallsCount * RENDER_CALL_DATA_SIZE * 4, vertexBufferData);

    // Bind indexes buffer and define attributes
    BindIndexBuffer(indexBufferID);

    // Vertex positions
    EnableVertexFloatAttribute(0, 3, RENDER_CALL_DATA_SIZE, 0);
    // Vertex color
    EnableVertexFloatAttribute(1, 4, RENDER_CALL_DATA_SIZE, 3);
    // Texture coords
    EnableVertexFloatAttribute(2, 2, RENDER_CALL_DATA_SIZE, 7);
    // Texture ID
    EnableVertexFloatAttribute(3, 1, RENDER_CALL_DATA_SIZE, 9);

    UnbindVetexBuffer();

    // Bind textures
    for (int i = 0; i < texturesCount; ++i)
        BindTexture(textureIDs[i], i);

    // Draw
    DrawTriangles(renderCallsCount * 6);

    UnbindVetexArray();

    UnbindTexture();
    DeleteVetexArray(vertexArrayID);
}

int Renderer::FindTextureSlot(uint32_t textureID)
{
    if (renderCallsCount >= MAX_RENDER_CALLS)
    {
        EndBatch();
        StartBatch();
    }

    int textureIDIndex = -1;
    for (int i = 0; i < texturesCount; ++i)
    {
        if (textureIDs[i] == textureID)
        {
            textureIDIndex = i;
            break;
        }
    }

    if (textureIDIndex == -1)
    {
        if (texturesCount >= MAX_TEXTURE_SLOTS)
        {
            EndBatch();
            StartBatch();
        }

        textureIDIndex = texturesCount;
        textureIDs[textureIDIndex] = textureID;
        texturesCount++;
    }

    return textureIDIndex;
}
