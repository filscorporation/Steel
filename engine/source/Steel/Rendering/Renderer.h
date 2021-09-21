#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "QuadRenderer.h"
#include "RendererData.h"

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

private:

    // Renderer state
    static glm::mat4 currentViewProjection;
    static ResourceID lastMaterial;
    static size_t lastPropertyBlockHash;

    static std::vector<Shader*> shadersUsed;
};
