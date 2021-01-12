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
    ResourcesManager* GetResourcesManager();
    Scene* GetCurrentScene();

private:
    void Init(ApplicationSettings settings);
};