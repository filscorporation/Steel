#pragma once

#include "glm/glm.hpp"

class Window
{
public:
    Window() = default;
    virtual ~Window() = default;

    virtual void Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync) = 0;
    virtual void InitInputHandler() = 0;
    virtual void Apply() = 0;

    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual bool GetIsMinimized() = 0;

    virtual bool IsScreenSizeDirty() = 0;
    virtual void SetScreenSizeDirty() = 0;

    virtual void UpdateSize() = 0;
    virtual void SwapBuffers() = 0;
    virtual void ClearDirty() = 0;
    virtual bool WindowShouldClose() = 0;
    virtual void Terminate() = 0;

    virtual float InvertY(float y) = 0;
};
