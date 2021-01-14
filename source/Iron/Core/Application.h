#pragma once

#include "../Scene/Object.h"
#include "../Scene/Scene.h"
#include "ResourcesManager.h"

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
    void Quit();
    ResourcesManager* GetResourcesManager();
    Scene* GetCurrentScene();

private:
    bool isRunning;

    void Init(ApplicationSettings settings);
};