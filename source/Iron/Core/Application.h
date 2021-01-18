#pragma once

#include "../Scene/Object.h"
#include "../Scene/Scene.h"
#include "ResourcesManager.h"
#include "../Rendering/Screen.h"

namespace ApplicationStates
{
    enum ApplicationState
    {
        Initializing,
        OnUpdate,
        OnLateUpdate,
        OnRender,
        CleaningDestroyedObjects,
    };
}

struct ApplicationSettings
{
    int ScreenWidth = 800;
    int ScreenHeight = 600;
    glm::vec3 ScreenColor = glm::vec3(0.2f, 0.3f, 0.3f);
    bool Fullscreen = false;
};

class Application
{
public:
    static Application* Instance;

    Application(ApplicationSettings settings);
    void Run();
    void Quit();
    ApplicationStates::ApplicationState State();
    ResourcesManager* GetResourcesManager();
    Scene* GetCurrentScene();

private:
    bool isRunning;
    ApplicationStates::ApplicationState state;

    void Init(ApplicationSettings settings);
    void Terminate();
};
