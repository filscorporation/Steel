#pragma once

#include "RenderList.h"

class RenderTask;

class RenderContext
{
public:
    RenderList List;
    RenderTask* Task = nullptr;
};
