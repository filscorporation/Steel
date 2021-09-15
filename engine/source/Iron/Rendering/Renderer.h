#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "QuadRenderer.h"
#include "RendererData.h"

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
    static void Draw(const QuadRenderer& quad);

    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
    static void SetDrawMode(DrawModes::DrawMode drawMode);

    static bool DrawWireframe;

private:

    // Renderer state
    static glm::mat4 currentViewProjection;
    static ResourceID lastMaterial;
    static size_t lastPropertyBlockHash;

    static std::vector<Shader*> shadersUsed;
    static DrawModes::DrawMode currentDrawMode;
};
