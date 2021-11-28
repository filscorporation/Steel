#pragma once

#include "RendererEnums.h"

class DrawCallsSource;

struct RenderTask
{
    DrawCallsSource* Source = nullptr;
    Framebuffer* TargetFramebuffer = nullptr;
    glm::mat4 ViewProjection {};
    ClearFlags::ClearFlag ClearFlagsValue = ClearFlags::All;
};
