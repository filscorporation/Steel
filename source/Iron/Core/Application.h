#pragma once

#include "../Scene/Object.h"
#include "../Scene/Scene.h"

struct ApplicationSettings
{
    int ScreenWidth = 800;
    int ScreenHeight = 600;
    bool Fullscreen = false;
};

class Application
{
public:
    static Application* Instance;

    Application(ApplicationSettings settings);
    void Run();
    Scene* GetCurrentScene();

private:
    void Init(ApplicationSettings settings);
};