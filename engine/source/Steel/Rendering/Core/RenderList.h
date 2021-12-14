#pragma once

#include "DrawCall.h"

#include <vector>

class RenderContext;

class RenderList
{
public:
    void Reserve(uint32_t size);
    void AddDrawCall(const DrawCall& drawCall);
    void SortDrawCalls(RenderContext* renderContext);
    void ExecuteDrawCalls(RenderContext* renderContext);
private:

    static void Clear(ClearFlags::ClearFlag clearFlag);
    static bool CanBatch(const DrawCall& drawCall1, const DrawCall& drawCall2);
    void PrepareBuffers();
    void ClearBuffers();
    void PrepareResources(RenderContext* renderContext);
    void ClearResources();

    std::vector<DrawCall> list;
    std::vector<uint32_t> sortingIndices;

    std::vector<Shader*> shadersUsed;
    float* vertexBufferData;
    uint32_t* indexBufferData;
    uint32_t indexBufferID, vertexBufferID;

    Shader* wireframeShader;
    Material* wireframeMaterial;
};
