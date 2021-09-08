#pragma once

#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourcesManager.h"

struct ScreenParameters
{
    bool CanResize;
    bool Fullscreen;
    int Width;
    int Height;
    int ResolutionX;
    int ResolutionY;
    int OffsetX;
    int OffsetY;
    glm::vec3 Color;
};

struct ApplicationSettings
{
    int ScreenWidth = 800;
    int ScreenHeight = 600;
    glm::vec3 ScreenColor = glm::vec3(0.2f, 0.3f, 0.3f);
    bool Fullscreen = false;
    bool DoubleBuffer = false;
};

struct ApplicationContext
{
    ResourcesManager* Resources;
    SceneManager* Scenes;
    ScreenParameters ScreenParameters;
};

class Application
{
public:
    static Application* Instance;
    static const ApplicationContext* Context();

    Application();
    virtual void Init(ApplicationSettings settings);
    void Run();
    virtual void RunUpdate();
    void Quit();

    ScreenParameters& ScreenParametersForUpdate();
    ResourcesManager* GetResourcesManager();
    SceneManager* GetSceneManager();
    Scene* GetCurrentScene();

    std::string GetRuntimePath();
    std::string GetDataPath();

protected:
    bool IsInitialized = false;
    bool IsRunning;
    ApplicationContext* CurrentContext = nullptr;
    ApplicationContext* AppContext = nullptr;

    virtual void Terminate();
    void InitSystems(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer);
    ApplicationContext* CreateContext(ApplicationSettings settings);
};
