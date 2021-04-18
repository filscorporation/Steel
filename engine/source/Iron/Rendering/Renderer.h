#pragma once

#include <glm/glm.hpp>

#include "RendererData.h"
#include "Camera.h"

namespace DrawModes
{
    enum DrawMode
    {
        Wireframe = 0,
        Normal = 1,
        Text = 2,
    };
}

class Renderer
{
public:
    static void Init();
    static void Terminate();

    static void OnBeforeRender(Camera& camera);
    static void OnAfterRender();
    static void Clear(glm::vec3 color);
    static void PrepareUIRender();
    static void DrawQuad(glm::vec3 vertices[4], glm::vec2 textureCoords[4], const glm::vec4& color, uint32_t textureID);

    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
    static void SetDrawMode(DrawModes::DrawMode drawMode);

    static bool DrawWireframe;

    static int DrawCallsStats;
    static int VerticesStats;

private:
    static int FindTextureSlot(uint32_t textureID);

    static DrawModes::DrawMode currentDrawMode;
};
