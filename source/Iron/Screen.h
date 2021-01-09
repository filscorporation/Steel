#pragma once

class Application;

class Screen
{
public:
    static int Width();
    static int Height();

    static void SwapBuffers();
    static bool WindowShouldClose();
    static void Terminate();

private:
    static void Init(int width, int height, bool fullscreen);

    friend class Application;
};