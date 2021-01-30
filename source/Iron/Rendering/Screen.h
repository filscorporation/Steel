#pragma once

#include "glm/glm.hpp"

class Application;

class Screen
{
public:
    static int GetWidth();
    static int GetHeight();
    static glm::vec3 Color();
    static glm::vec2 ScreenToWorldPosition(glm::vec2 position);

    static void SwapBuffers();
    static bool WindowShouldClose();
    static void Terminate();

private:
    static void Init(int width, int height, glm::vec3 color, bool fullscreen);

    friend class Application;
};