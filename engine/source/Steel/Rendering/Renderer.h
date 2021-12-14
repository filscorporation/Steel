#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Rendering/MaterialSystem/Shader.h"
#include "Core/RenderTask.h"

class Renderer
{
public:
    static void Init();
    static void Terminate();

    static void Draw(RenderTask& renderTask);
};
