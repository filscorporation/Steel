#include "RenderList.h"
#include "RenderContext.h"
#include "Steel/Core/Application.h"
#include "OpenGLAPI.h"
#include "Steel/Rendering/Core/RenderTask.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#define VIEW_PROJECTION "view_projection"

#define MAX_VB_SIZE 100000
#define MAX_IB_SIZE 100000

void RenderList::AddDrawCall(const DrawCall& drawCall)
{
    list.emplace_back(drawCall);
}

void RenderList::SortDrawCalls(RenderContext* renderContext)
{
    struct
    {
        bool operator()(DrawCall& a, DrawCall& b) const
        { return a.Queue > b.Queue || (a.Queue == b.Queue && a.SortingOrder > b.SortingOrder); }
    } comparer;

    std::sort(list.begin(), list.end(), comparer);
}

void RenderList::ExecuteDrawCalls(RenderContext* renderContext)
{
    PrepareBuffers();

    for (int i = 0; i < list.size(); ++i)
    {
        auto& drawCall = list[i];

        if (drawCall.VB.IsEmpty() || drawCall.IB.IsEmpty()
            || drawCall.RenderMaterial == nullptr || drawCall.RenderMaterial->MainShader == nullptr)
            continue;

        // Setup material
        drawCall.RenderMaterial->MainShader->Use();
        drawCall.RenderMaterial->Properties.Apply(drawCall.RenderMaterial->MainShader);
        drawCall.CustomProperties.Apply(drawCall.RenderMaterial->MainShader);

        if (!drawCall.RenderMaterial->MainShader->GlobalUniformsSet)
        {
            shadersUsed.push_back(drawCall.RenderMaterial->MainShader);
            drawCall.RenderMaterial->MainShader->GlobalUniformsSet = true;
            drawCall.RenderMaterial->Properties.SetMat4(VIEW_PROJECTION, glm::value_ptr(renderContext->Task->ViewProjection));
        }

        // Try batch
        int batchSize = 1;
        uint32_t vbSize = drawCall.VB.Size;
        uint32_t ibSize = drawCall.IB.Size;
        for (int j = i + 1; j < list.size(); ++j)
        {
            vbSize += list[j].VB.Size;
            ibSize += list[j].IB.Size;
            if (!CanBatch(drawCall, list[j]) || vbSize >= MAX_VB_SIZE || ibSize >= MAX_IB_SIZE)
                break;
            batchSize++;
        }

        uint32_t verticesCount = 0;
        vbSize = ibSize = 0;
        if (batchSize > 1)
        {
            // Fill batch data
            for (int j = 0; j < batchSize; ++j)
            {
                std::copy(list[i + j].VB.Data, list[i + j].VB.Data + list[i + j].VB.Size, vertexBufferData + vbSize);
                std::copy(list[i + j].IB.Data, list[i + j].IB.Data + list[i + j].IB.Size, indexBufferData + ibSize);
                // We can't just copy index buffer, need to add vertices count as step
                for (int k = 0; k < list[i + j].IB.Size; ++k)
                    indexBufferData[ibSize + k] += verticesCount;
                vbSize += list[i + j].VB.Size;
                ibSize += list[i + j].IB.Size;
                verticesCount += list[i + j].VB.VerticesCount;
            }
        }
        else
        {
            vbSize = drawCall.VB.Size;
            ibSize = drawCall.IB.Size;
            verticesCount = drawCall.VB.VerticesCount;
        }

        // Prepare draw
        uint32_t vertexArrayID = OpenGLAPI::GenerateVertexArray();
        OpenGLAPI::BindVertexArray(vertexArrayID);

        // Set vertices and indices data
        OpenGLAPI::BindVertexBuffer(vertexBufferID);
        OpenGLAPI::BindIndexBuffer(indexBufferID);

        if (batchSize == 1)
        {
            OpenGLAPI::SetVertexBufferSubData(drawCall.VB.Size, drawCall.VB.Data);
            OpenGLAPI::SetIndexBufferSubData(drawCall.IB.Size, drawCall.IB.Data);
        }
        else
        {
            OpenGLAPI::SetVertexBufferSubData(vbSize, vertexBufferData);
            OpenGLAPI::SetIndexBufferSubData(ibSize, indexBufferData);
        }

        // TODO: check if valid in batch
        uint32_t blockSize = 0;
        for (int j = 0; j < drawCall.VB.AttributeBlocks.size(); ++j)
        {
            OpenGLAPI::EnableVertexFloatAttribute(j, drawCall.VB.AttributeBlocks[j], drawCall.VB.BlockSize, blockSize);
            blockSize += drawCall.VB.AttributeBlocks[j];
        }

        // Draw
        OpenGLAPI::DrawTriangles(ibSize);
        i += batchSize - 1;

        OpenGLAPI::UnbindVertexArray();
        OpenGLAPI::DeleteVertexArray(vertexArrayID);

        Application::Context()->Stats.VerticesCount += verticesCount;
        Application::Context()->Stats.DrawCalls ++;
    }

    // Clear
    for (auto& shader : shadersUsed)
        shader->GlobalUniformsSet = false;
    shadersUsed.clear();

    ClearBuffers();
}

bool RenderList::CanBatch(const DrawCall& drawCall1, const DrawCall& drawCall2)
{
    return drawCall1.RenderMaterial->ID == drawCall2.RenderMaterial->ID
        && drawCall1.CustomProperties.GetHash() == drawCall2.CustomProperties.GetHash();
}

void RenderList::PrepareBuffers()
{
    // Reserve space for all vertex data
    vertexBufferData = new float[MAX_VB_SIZE];
    vertexBufferID = OpenGLAPI::GenerateVertexBuffer(MAX_VB_SIZE, nullptr, OpenGLAPI::Dynamic);

    // Reserve space for all index data
    indexBufferData = new uint32_t[MAX_IB_SIZE];
    indexBufferID = OpenGLAPI::GenerateIndexBuffer(MAX_IB_SIZE, nullptr, OpenGLAPI::Dynamic);
}

void RenderList::ClearBuffers()
{
    OpenGLAPI::DeleteBuffer(indexBufferID);
    OpenGLAPI::DeleteBuffer(vertexBufferID);

    delete[] vertexBufferData;
    delete[] indexBufferData;
    vertexBufferData = nullptr;
    indexBufferData = nullptr;
}
