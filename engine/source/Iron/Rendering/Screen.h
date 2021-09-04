#pragma once

#include "glm/glm.hpp"

#include "Framebuffer.h"

class Application;

#define UI_MAX_DISTANCE 1.0f

class Screen
{
public:
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

    static void UpdateSize();
    static void SwapBuffers();
    static bool WindowShouldClose();
    static void Terminate();

    static void EnterCallback();
    static void ExitCallback();
    static bool IsInCallback();

private:
    static void Init(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer);
    static void Apply();

    static void UpdateUIViewProjection();

    static Framebuffer* _framebuffer;
    static int _width, _height;
    static int _xPosition, _yPosition;
    static glm::vec3 _color;
    static bool _fullscreen;
    static bool _isMinimized;
    static bool _doubleBuffer;
    static bool isInResizeCallback;
    static glm::mat4 _viewProjection;

    friend class Application;
};