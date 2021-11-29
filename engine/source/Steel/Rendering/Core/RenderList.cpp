#include "RenderList.h"
#include "RenderContext.h"
#include "OpenGLAPI.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Math/Math.h"
#include "Steel/Math/Sorting.h"
#include "Steel/Rendering/Core/RenderTask.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#define VIEW_PROJECTION "view_projection"

#define MAX_VB_SIZE 100000
#define MAX_IB_SIZE 100000

void RenderList::Reserve(uint32_t size)
{
    list.reserve(size);
}

void RenderList::AddDrawCall(const DrawCall& drawCall)
{
    list.emplace_back(drawCall);
}

void RenderList::SortDrawCalls(RenderContext* renderContext)
{
    std::vector<uint32_t> sortingKeys;
    sortingKeys.resize(list.size());
    for (int i = 0; i < sortingKeys.size(); ++i)
    {
        uint32_t key = ((uint32_t)(list[i].Queue) << 31) | (Math::FloatToUnsigned(list[i].SortingOrder) >> 1);
        sortingKeys[i] = key;
    }

    sortingIndices.clear();
    sortingIndices.resize(list.size());
    for (int i = 0; i < sortingIndices.size(); ++i)
        sortingIndices[i] = i;

    Sorting::RadixSort(sortingKeys, sortingIndices);
}

void RenderList::ExecuteDrawCalls(RenderContext* renderContext)
{
    PrepareBuffers();

    for (int i = 0; i < list.size(); ++i)
    {
        auto& drawCall = list[sortingIndices[i]];

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
            vbSize += list[sortingIndices[j]].VB.Size;
            ibSize += list[sortingIndices[j]].IB.Size;
            if (!CanBatch(drawCall, list[sortingIndices[j]]) || vbSize >= MAX_VB_SIZE || ibSize >= MAX_IB_SIZE)
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
                auto& batchDrawCall = list[sortingIndices[i + j]];
                std::copy(batchDrawCall.VB.Data, batchDrawCall.VB.Data + batchDrawCall.VB.Size, vertexBufferData + vbSize);
                std::copy(batchDrawCall.IB.Data, batchDrawCall.IB.Data + batchDrawCall.IB.Size, indexBufferData + ibSize);
                // We can't just copy index buffer, need to add vertices count as step
                for (int k = 0; k < batchDrawCall.IB.Size; ++k)
                    indexBufferData[ibSize + k] += verticesCount;
                vbSize += batchDrawCall.VB.Size;
                ibSize += batchDrawCall.IB.Size;
                verticesCount += batchDrawCall.VB.VerticesCount;
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
        for (auto& attribute : drawCall.VB.Attributes)
        {
            OpenGLAPI::EnableVertexFloatAttribute(attribute.AttributeID, attribute.Size, drawCall.VB.BlockSize, blockSize);
            blockSize += attribute.Size;
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
