#pragma once

#include "RendererEnums.h"

class DrawCallsSource;
class Framebuffer;

namespace RenderTaskModes
{
    enum RenderTaskMode
    {
        Normal = 0,
        Wireframe = 1,
        Mixed = 2,
    };
}

struct RenderTask
{
    DrawCallsSource* Source = nullptr;
    Framebuffer* TargetFramebuffer = nullptr;
    glm::mat4 ViewProjection {};
    ClearFlags::ClearFlag ClearFlagsValue = ClearFlags::All;
    RenderTaskModes::RenderTaskMode RenderMode = RenderTaskModes::Normal;
};
