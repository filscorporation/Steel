#pragma once

#include "Framebuffer.h"
#include "Window.h"

#include "glm/glm.hpp"

class Screen
{
public:
    static void Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync);
    static void InitInputHandler();
    static void Apply();

    static Framebuffer* ScreenFramebuffer();
    static int GetWidth();
    static void SetWidth(int width);
    static int GetHeight();
    static void SetHeight(int width);
    static bool GetFullscreen();
    static void SetFullscreen(bool fullscreen);
    static bool GetIsMinimized();
    static glm::vec3 GetColor();
    static void SetColor(glm::vec3 color);
    static glm::mat4 GetUIViewProjection();

    static bool IsScreenSizeDirty();
    static void SetScreenSizeDirty();

    static void UpdateSize();
    static void SwapBuffers();
    static bool WindowShouldClose();
    static void Terminate();

    static glm::vec2 Transform(const glm::vec2& position);
    static float InvertY(float y);

    static void EnterCallback();
    static void ExitCallback();
    static bool IsInCallback();

private:
    static Window* CreateWindow();

    static Window* _window;

    static Framebuffer* _framebuffer;
    static bool _doubleBuffer;
    static bool isInResizeCallback;
};