#pragma once

#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Rendering/MaterialSystem/MaterialPropertyBlock.h"
#include "Steel/Rendering/Core/VertexBuffer.h"
#include "Steel/Rendering/Core/IndexBuffer.h"

namespace RenderingQueue
{
    enum RenderingQueue
    {
        Opaque      = 0,
        Transparent = 1,
    };
}

struct DrawCall
{
    VertexBuffer VB {};
    IndexBuffer IB {};

    Material* RenderMaterial = nullptr;
    MaterialPropertyBlock CustomProperties {};

    float SortingOrder = 0;
    RenderingQueue::RenderingQueue Queue = RenderingQueue::Opaque;
};
